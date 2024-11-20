// Draggable modal dialog
class EditorModal: ScriptView
{
	protected float m_DragOffsetX, m_DragOffsetY;
	TextWidget TitleText;
	Widget WindowDragWrapper;
	
	void EditorModal()
	{
		if (TitleText) {
			TitleText.SetText(GetTitle());
		}
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		if (w == WindowDragWrapper) {
			m_LayoutRoot.GetPos(m_DragOffsetX, m_DragOffsetY);
			m_DragOffsetX -= x; m_DragOffsetY -= y;		
			g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DragUpdate);	
		}
		
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever) 
	{ 
		return true; 
	}
	
	private bool DragUpdate()
	{
		int x, y;
		GetMousePos(x, y);
		m_LayoutRoot.SetPos(x + m_DragOffsetX, y + m_DragOffsetY);
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Remove(DragUpdate);
		
		if (w == WindowDragWrapper) {
			m_LayoutRoot.SetPos(x + m_DragOffsetX, y + m_DragOffsetY);
	    }		
		
		return false;
	}
	
	string GetTitle()
	{
		return "Dialog";
	}
}

class EditorFileDialogController: ViewController
{
	ref ObservableCollection<ref EditorFileView> Files = new ObservableCollection<ref EditorFileView>(this);
	ref ObservableCollection<ref EditorDirectoryView> Directories = new ObservableCollection<ref EditorDirectoryView>(this);
	ref ObservableCollection<ref EditorFileQuickView> FolderViews = new ObservableCollection<ref EditorFileQuickView>(this);
}

enum eDialogMode
{
	OPEN = 1,
	SAVE = 2,
	EXPORT = 3,
	IMPORT = 4,
	NEW
}

enum eDialogFlags
{
	WARN_ON_OVERWRITE = 1,
	ALLOW_EMPTY_FILES = 2,
	ALLOW_DOUBLE_CLICK = 4
}

class EditorFileDialog: EditorModal
{
	static const ref array<string> AVAILABLE_DIRECTORIES = {
		SystemPath.Saves(),
		SystemPath.Profile(),
//		SystemPath.Mission()
	};

	protected EditorFileDialogController m_TemplateController;
	
	protected eDialogMode m_DialogMode;
	protected eDialogFlags m_DialogFlags;
	protected int m_CurrentHistoryIndex = 0;
	protected ref array<string> m_DirectoryHistory = {};
	protected string m_CurrentDirectory, m_CurrentFile;
	protected ref ScriptCaller m_ScriptCallback;
	protected ref EditorFileType m_FileType;
	protected ref EditorMessageBox m_EditorMessageBox;
	
	EditBoxWidget SearchBox, FileNameBox;
	ButtonWidget SaveButton, CancelButton, ExplorerBack, ExplorerFwd, TitleClose, RefreshButton;
	ImageWidget ExplorerBackImage, ExplorerFwdImage;

	void EditorFileDialog(typename file_type, ScriptCaller on_file_selected, eDialogMode dialog_mode, eDialogFlags dialog_flags)
	{
		m_TemplateController = EditorFileDialogController.Cast(m_Controller);
		m_FileType = EditorFileType.Cast(file_type.Spawn());
		m_ScriptCallback = on_file_selected;
		m_DialogMode = dialog_mode;
		m_DialogFlags = dialog_flags;

		if (!file_type.IsInherited(EditorFileType) || !m_FileType) {
			Error("invalid file_type parameter");
			Delete();
			return;
		}

		foreach (string directory: AVAILABLE_DIRECTORIES) {
			m_TemplateController.Directories.Insert(new EditorDirectoryView(directory, ScriptCaller.Create(OnDirectoryPressed)));
		}
		
		switch (m_DialogMode) {
			case eDialogMode.OPEN: {
				SaveButton.SetText("#STR_EDITOR_OPEN");
				break;
			}
			
			case eDialogMode.SAVE: {
				SaveButton.SetText("#STR_EDITOR_SAVE");
				break;
			}
			
			case eDialogMode.IMPORT: {
				SaveButton.SetText("#STR_EDITOR_IMPORT");
				break;
			}
			
			case eDialogMode.EXPORT: {
				SaveButton.SetText("#STR_EDITOR_EXPORT");
				break;
			}
			
			case eDialogMode.NEW: {
				SaveButton.SetText("#STR_EDITOR_NEW");
				break;
			}
		}
		
		m_TemplateController.Directories[0].GetLayoutRoot().SetColor(0xff007acc);

		SetDirectory(SystemPath.Combine(SystemPath.Saves(), "Editor"));
	}

	override void Update(float dt)
	{
		bool is_forward_disabled = m_CurrentHistoryIndex == 0;
		ExplorerFwd.Enable(!is_forward_disabled);
		if (is_forward_disabled) {
			ExplorerFwdImage.SetAlpha(100 / 255);
		} else {
			ExplorerFwdImage.SetAlpha(1.0);
		}

		bool is_backward_disabled = (m_CurrentHistoryIndex >= m_DirectoryHistory.Count() - 1);
		ExplorerBack.Enable(!is_backward_disabled);
		if (is_backward_disabled) {
			ExplorerBackImage.SetAlpha(100 / 255);
		} else {
			ExplorerBackImage.SetAlpha(1.0);
		}

	}

	protected void SetDirectory(string directory, bool update_history = true)
	{
		if (!FileExist(directory)) {
			Error(string.Format("Attempted to browse to invalid directory: %1", directory));
			return;
		}
		
		if (directory == m_CurrentDirectory) {
			update_history = false;
		}
		
		m_CurrentDirectory = SystemPath.Format(directory);

		array<ref Param2<string, string>> extensions = {};
		m_FileType.GetValidExtensions(extensions);

		string search_box_text = SearchBox.GetText();
		if (search_box_text == "Search..." || search_box_text == "") {
			search_box_text = "*";
		}
		
		search_box_text.Replace(" ", "*");
		
		array<string> files = {};
		foreach (auto extension: extensions) {
			array<string> files_temp = Directory.EnumerateFiles(m_CurrentDirectory, extension.param2, 0);
			foreach (string file_temp: files_temp) {
				if (files.Find(file_temp) == -1 && File.WildcardMatch(file_temp, search_box_text)) {
					files.Insert(file_temp);
				}
			}
		}

		array<string> folders = Directory.EnumerateDirectories(m_CurrentDirectory);

		folders.Sort();
		files.Sort();
		array<string> all_loaded_files = {};
		all_loaded_files.InsertAll(folders);
		all_loaded_files.InsertAll(files);

		m_TemplateController.Files.Clear();
		foreach (string sorted_file: all_loaded_files) {
			m_TemplateController.Files.Insert(new EditorFileView(sorted_file, ScriptCaller.Create(OnFilePressed), ScriptCaller.Create(OnFileDoublePressed)));
		}

		array<string> directory_split = {};
		m_CurrentDirectory.Split(SystemPath.SEPERATOR, directory_split);
		m_TemplateController.FolderViews.Clear();
		for (int j = 0; j < directory_split.Count(); j++) {
			string full_dir = directory_split[0];
			for (int k = 1; k <= j; k++) {
				full_dir = SystemPath.Join(full_dir, directory_split[k]);
			}

			m_TemplateController.FolderViews.Insert(new EditorFileQuickView(full_dir, ScriptCaller.Create(OnQuickDirectoryPressed)));
		}

		if (update_history) {
			for (int i = m_DirectoryHistory.Count() - m_CurrentHistoryIndex; i < m_DirectoryHistory.Count(); i++) {
				m_DirectoryHistory.RemoveOrdered(i);
			}
	
			m_DirectoryHistory.Insert(directory);
			m_CurrentHistoryIndex = 0;
		}
				
		PrintFormat("Loaded Directory %1, %2 folders, %3 files", m_CurrentDirectory, folders.Count(), files.Count());
	}

	protected void OnFilePressed(EditorFileView view, string file)
	{
		for (int i = 0; i < m_TemplateController.Files.Count(); i++) {
			 m_TemplateController.Files[i].GetLayoutRoot().SetColor(0xff24282e);
		}
		
		m_CurrentFile = file;
		view.GetLayoutRoot().SetColor(0xff007acc);
		FileNameBox.SetText(File.GetName(m_CurrentFile));
	}

	protected void OnFileDoublePressed(EditorFileView view, string file)
	{
		bool is_directory = file.LastIndexOf(".") != file.Length() - 4;
		if (is_directory) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetDirectory, 0, 0, file, true);
		} else {
			if (m_DialogFlags & eDialogFlags.ALLOW_DOUBLE_CLICK) {
				if (m_ScriptCallback) {
					m_ScriptCallback.Invoke(file);
					Delete();
				}
			}			
		}
	}

	protected void OnQuickDirectoryPressed(EditorFileQuickView view, string directory)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetDirectory, 0, 0, directory, true);
	}

	protected void OnDirectoryPressed(EditorDirectoryView view, string directory)
	{
		for (int i = 0; i < m_TemplateController.Directories.Count(); i++) {
			 m_TemplateController.Directories[i].GetLayoutRoot().SetColor(0xff24282e);
		}
		
		view.GetLayoutRoot().SetColor(0xff007acc);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetDirectory, 0, 0, directory, true);
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (w) {
			case SearchBox: {
				SearchBox.SetText(string.Empty);
				break;
			}

			case FileNameBox: {
				FileNameBox.SetText(string.Empty);
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == 0) {
			switch (w) {
				case RefreshButton: {
					SetDirectory(m_CurrentDirectory, false);
					break;
				}
				
				case ExplorerBack: {
					m_CurrentHistoryIndex++;
					string last_directory = m_DirectoryHistory[m_DirectoryHistory.Count() - m_CurrentHistoryIndex - 1];
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetDirectory, 0, 0, last_directory, false);
					
					break;
				}
				
				case ExplorerFwd: {
					m_CurrentHistoryIndex--;
					string fwd_dir = m_DirectoryHistory[m_DirectoryHistory.Count() - m_CurrentHistoryIndex - 1];
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetDirectory, 0, 0, fwd_dir, false);
					break;
				}
				
				case SaveButton: {
					if (!FileNameBox.GetText() && !(m_DialogFlags & eDialogFlags.ALLOW_EMPTY_FILES)) {
						m_EditorMessageBox = new EditorMessageBox("Please select a valid file name", MessageBoxButtons.OK, null);
						return true;
					}

					string final_file = SystemPath.Combine(m_CurrentDirectory, FileNameBox.GetText());
					if (FileExist(final_file) && (m_DialogFlags & eDialogFlags.WARN_ON_OVERWRITE)) {
						string msg = string.Format("Overwrite File %1?", final_file);
						m_EditorMessageBox = new EditorMessageBox(msg, MessageBoxButtons.OKCancel, ScriptCaller.Create(ForceOverwriteFileFromTextBox));
						return true;
					}

					if (m_ScriptCallback) {
						m_ScriptCallback.Invoke(final_file);
						Delete();
						return true; 
					}
					
					break;
				}
				
				case TitleClose:
				case CancelButton: {
					Delete();
					return true;
				}
			}
		}
		
		return super.OnClick(w, x, y, button);
	}

	protected void ForceOverwriteFileFromTextBox(DialogResult result)
	{
		if (result == DialogResult.OK) {
			string final_file = SystemPath.Combine(m_CurrentDirectory, FileNameBox.GetText());
			if (m_ScriptCallback) {
				m_ScriptCallback.Invoke(final_file);
			}
			
			Delete();
		}
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		switch (w) {
			case SearchBox: {
				// for now
				SetDirectory(m_CurrentDirectory);
				return true;
			}
		}

		return super.OnChange(w, x, y, finished);
	}	
	
	EditorFileDialogController GetTemplateController()
	{
		return m_TemplateController;
	}
		
	override typename GetControllerType()
	{
		return EditorFileDialogController;
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/FileDialog.layout";
	}

	override string GetTitle()
	{
		return "File Explorer";
	}
}