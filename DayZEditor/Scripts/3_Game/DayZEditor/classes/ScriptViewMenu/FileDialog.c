class EditorFileDialogController: ViewController
{
	ref ObservableCollection<ref EditorFileView> Files = new ObservableCollection<ref EditorFileView>(this);
	ref ObservableCollection<ref EditorDirectoryView> Directories = new ObservableCollection<ref EditorDirectoryView>(this);
	ref ObservableCollection<ref EditorFileQuickView> FolderViews = new ObservableCollection<ref EditorFileQuickView>(this);
}

class EditorFileView: ScriptView
{
	protected string m_File;
	protected ref ScriptCaller m_OnClicked, m_OnDoubleClicked;

	protected float m_ClickTick;
	
	ImageWidget Icon;
	TextWidget FileName, Extension;

	void EditorFileView(string file, ScriptCaller on_click, ScriptCaller on_double_click)
	{
		m_File = file;
		m_OnClicked = on_click;
		m_OnDoubleClicked = on_double_click;

		string file_name = File.GetName(m_File);
		string file_extension = File.GetExtension(m_File);
		FileName.SetText(file_name);
		Extension.SetText(file_extension);
		// Set icon to folder
		if (file_extension == string.Empty) {
			Symbols.FOLDER.Load(Icon, 3);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (m_OnClicked) {
			m_OnClicked.Invoke(this, m_File);
		}
		
		if (GetGame().GetTickTime() < m_ClickTick + 0.3) {
			if (m_OnDoubleClicked) {
				m_OnDoubleClicked.Invoke(this, m_File);
			}
			m_ClickTick = 0;
		} else {
			m_ClickTick = GetGame().GetTickTime();
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		return super.OnMouseButtonUp(w, x, y, button);
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/File.layout";
	}
}

class EditorDirectoryView: ScriptView
{
	protected string m_Directory;
	protected ref ScriptCaller m_OnClicked;
	TextWidget Text;

	void EditorDirectoryView(string directory, ScriptCaller on_click)
	{
		m_Directory = directory;
		m_OnClicked = on_click;
		
		Text.SetText(m_Directory);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (m_OnClicked) {
			m_OnClicked.Invoke(this, m_Directory);
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		return super.OnMouseButtonUp(w, x, y, button);
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/Directory.layout";
	}
}

class EditorFileQuickView: ScriptView
{
	protected string m_FullPath;
	TextWidget Text;

	protected ref ScriptCaller m_OnClicked;

	void EditorFileQuickView(string full_path, ScriptCaller on_clicked)
	{
		m_FullPath = full_path;
		m_OnClicked = on_clicked;
		
		string text = File.GetName(full_path);
		if (!text) {
			text = full_path;
		}
		
		Text.SetText(text);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (m_OnClicked) {
			m_OnClicked.Invoke(this, m_FullPath);
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/FileDialogFolderQuickView.layout";
	}
}

class EditorFileDialog: ScriptView
{
	static const ref array<string> AVAILABLE_DIRECTORIES = {
		SystemPath.Saves(),
		SystemPath.Profile(),
//		SystemPath.Mission()
	};

	protected EditorFileDialogController m_TemplateController;

	protected float m_DragOffsetX, m_DragOffsetY;
	protected int m_CurrentHistoryIndex = 0;
	protected ref array<string> m_DirectoryHistory = {};
	protected string m_CurrentDirectory, m_CurrentFile;
	protected ref ScriptCaller m_ScriptCallback;
	protected ref EditorFileType m_FileType;
	
	Widget WindowDragWrapper;
	EditBoxWidget SearchBox, FileNameBox;
	TextWidget TitleText;
	ButtonWidget SaveButton, CancelButton, ExplorerBack, ExplorerFwd, TitleClose, RefreshButton;
	ImageWidget ExplorerBackImage, ExplorerFwdImage;

	void EditorFileDialog(string title, typename file_type, ScriptCaller on_file_selected)
	{
		m_TemplateController = EditorFileDialogController.Cast(m_Controller);
		m_FileType = EditorFileType.Cast(file_type.Spawn());
		m_ScriptCallback = on_file_selected;

		if (!file_type.IsInherited(EditorFileType) || !m_FileType) {
			Error("invalid file_type parameter");
			Delete();
			return;
		}

		foreach (string directory: AVAILABLE_DIRECTORIES) {
			m_TemplateController.Directories.Insert(new EditorDirectoryView(directory, ScriptCaller.Create(OnDirectoryPressed)));
		}
		
		m_TemplateController.Directories[0].GetLayoutRoot().SetColor(0xff007acc);

		SetDirectory(SystemPath.Saves());
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
		
		m_CurrentDirectory = SystemPath.Format(directory);

		array<ref Param2<string, string>> extensions = {};
		m_FileType.GetValidExtensions(extensions);

		array<string> files = {};
		foreach (auto extension: extensions) {
			files.InsertAll(Directory.EnumerateFiles(m_CurrentDirectory, extension.param2, 0));
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
		directory_split.Debug();
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
		
		//Print(m_CurrentHistoryIndex);
		//m_DirectoryHistory.Debug();
		
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
			if (m_ScriptCallback) {
				m_ScriptCallback.Invoke(file);
				Delete();
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
					if (m_ScriptCallback && m_CurrentFile) {
						m_ScriptCallback.Invoke(m_CurrentFile);
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
}