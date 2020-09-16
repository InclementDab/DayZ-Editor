
class EditorFile
{
	string FileName;
	string FileDirectory;
	FileSearchMode FileAtrributes;
	
	void EditorFile(string name, string directory, FileSearchMode attributes) {
		FileName = name; FileDirectory = directory; FileAtrributes = attributes;
	}
	
	string GetFile() 
		return string.Format("%1\\%2", FileDirectory, FileName);
	
}


enum FileSearchMode {
	FOLDERS,
	FILES
};

class EditorFileDialog: EditorDialog
{
	protected string m_CurrentDirectory;
	protected string m_StartDirectory;
	protected TextListboxWidget m_FileHostListbox;
	
	void EditorFileDialog()
	{
		EditorLog.Trace("EditorFileDialog");
		
		
		Widget content = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorFileDialog.layout", null);
		AddContent(content);
		
		m_FileHostListbox = TextListboxWidget.Cast(m_LayoutRoot.FindAnyWidget("FolderHostListBox"));
	}
	
	private void LoadFiles(string directory, string filter, out ref array<ref EditorFile> folder_array, FileSearchMode search_mode)
	{
		TStringArray name_array = new TStringArray();
		string filename;
		FileAttr fileattr;
		
		if (search_mode == FileSearchMode.FOLDERS) {
			filter = "*";
		}
		
		FindFileHandle filehandle = FindFile(directory + filter, filename, fileattr, FindFileFlags.ALL);
		if ((fileattr & FileAttr.DIRECTORY) == FileAttr.DIRECTORY) {
			if (search_mode == FileSearchMode.FOLDERS) {
				name_array.Insert(filename + "\\");
			}
		} else {
			if (search_mode == FileSearchMode.FILES) {
				name_array.Insert(filename);
			}
		}
	
		
		while (FindNextFile(filehandle, filename, fileattr)) {
			if ((fileattr & FileAttr.DIRECTORY) == FileAttr.DIRECTORY) {
				if (search_mode == FileSearchMode.FOLDERS) {
					name_array.Insert(filename + "\\");
				}
			} else {
				if (search_mode == FileSearchMode.FILES) {
					name_array.Insert(filename);
				}
			}
		}
		CloseFindFile(filehandle);
		name_array.Sort();
		
		foreach (string sorted_name: name_array) {
			folder_array.Insert(new EditorFile(sorted_name, directory, search_mode));
		}
	}
	
	
	void LoadFileDirectory(string directory, string filter)
	{
		EditorLog.Trace("EditorFileDialog::LoadFileDirectory");
		m_CurrentDirectory = directory;
		string filterdir = string.Format("%1%2", directory, filter);
		Print(directory);
		m_FileHostListbox.ClearItems();
		ref array<ref EditorFile> editor_file_array = new array<ref EditorFile>();
		ref array<ref EditorFile> editor_folder_array = new array<ref EditorFile>();
		TStringArray file_array = new TStringArray();
		TStringArray folder_array = new TStringArray();
		
		LoadFiles(directory, filter, editor_folder_array, FileSearchMode.FOLDERS);
		LoadFiles(directory, filter, editor_file_array, FileSearchMode.FILES);
				

		foreach (EditorFile sorted_folder: editor_folder_array) {
			m_FileHostListbox.AddItem(sorted_folder.FileName, sorted_folder, 0);
		}
		
		foreach (EditorFile sorted_file: editor_file_array) {
			m_FileHostListbox.AddItem(sorted_file.FileName, sorted_file, 0);
		}
	}
	
	string ShowFileDialog(string start_directory = "$profile:\\")
	{
		m_StartDirectory = start_directory;
		
		Show();
		
		// return file directory
		return "";
	}

}


class EditorFileOpenDialog: EditorFileDialog
{
	
	void EditorFileOpenDialog()
	{
		EditorLog.Trace("EditorFileOpenDialog");
		
		AddButton("Open", "OpenCallback");
		AddButton("Cancel", "Close");
		
		string filter = "*.dze";
		
		LoadFileDirectory(m_StartDirectory, filter);
	}
	
	
	void ~EditorFileOpenDialog()
	{
		EditorLog.Trace("~EditorFileOpenDialog");
	}
	
	void OpenCallback()
	{
		EditorLog.Trace("EditorFileOpenDialog::OpenCallback");
		EditorFile data;
		m_FileHostListbox.GetItemData(m_FileHostListbox.GetSelectedRow(), 0, data);
		Close();
	}
		
	
	/*
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0) return false;
		EditorFile data;
		m_FileHostListbox.GetItemData(m_FileHostListbox.GetSelectedRow(), 0, data);
		
		if (data.FileAtrributes == FileSearchMode.FOLDERS) {
			string filter = "*.dze";
			LoadFileDirectory(data.GetFile(), filter);
			return true;
		}
		
		//GetEditor().Open(data.GetFile());
		Close();
		
		return true;
	}*/
}



class EditorFileImportDialog: EditorFileDialog
{
	
	void EditorFileImportDialog()
	{
		EditorLog.Trace("EditorFileImportDialog");
		
		AddButton("Import", "ImportCallback");
		AddButton("Cancel", "Close");
		
		string filter = "*.vpp";
		//string filter = "*";
		
		LoadFileDirectory(m_StartDirectory, filter);
		
	}
	
	
	void ~EditorFileImportDialog() {
		EditorLog.Trace("~EditorFileImportDialog");
	}
	
	
	void ImportCallback()
	{
		EditorLog.Trace("EditorFileImportDialog::ImportCallback");
		EditorFile data;
		m_FileHostListbox.GetItemData(m_FileHostListbox.GetSelectedRow(), 0, data);
		//GetEditor().Import(ImportMode.VPP, data.GetFile());
		Close();
	}


	/*
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0) return false;
		EditorFile data;
		m_FileHostListbox.GetItemData(m_FileHostListbox.GetSelectedRow(), 0, data);
		
		if (data.FileAtrributes == FileSearchMode.FOLDERS) {
			string filter = "*.vpp";
			LoadFileDirectory(data.GetFile(), filter);
			return true;
		}
		
		//GetEditor().Import(ImportMode.VPP, data.GetFile());
		Close();
		
		return true;
	}*/
}


class EditorFileSaveDialog: EditorFileDialog
{
	protected EditBoxWidget m_FileNameBox;
	
	protected EditorWorldData m_EditorWorldData;
	
	void EditorFileSaveDialog(EditorWorldData world_data)
	{
		EditorLog.Trace("EditorFileSaveDialog");
		m_EditorWorldData = world_data;
				
		AddButton("Save", "SaveCallback");
		AddButton("Cancel", "Close");
		
		Widget w = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorFileNameElement.layout", m_DialogController.ButtonGrid);
		m_FileNameBox = EditBoxWidget.Cast(w.FindAnyWidget("FileNameEditBox"));
		
		string default_name = "new";
		int i = 1;
		while (FileExist(m_StartDirectory + default_name)) {
			default_name = string.Format("new(%1)", i);
			i++;
		}
		
		m_FileNameBox.SetText(default_name);

		string filter = "*";
		LoadFileDirectory(m_StartDirectory, filter);
		
	}
	
	
	void ~EditorFileSaveDialog() {
		EditorLog.Trace("~EditorFileSaveDialog");
	}
	
	void SaveCallback()
	{
		GetEditor().Save(m_CurrentDirectory + m_FileNameBox.GetText(), m_EditorWorldData);
		
		Close();
	}
	
	/*
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorFileSaveDialog::OnClick");
		
		if (button != 0) return false; 
		

		if (w == m_FileHostListbox) {
			EditorFile data;
			m_FileHostListbox.GetItemData(m_FileHostListbox.GetSelectedRow(), 0, data);
			m_FileNameBox.SetText(data.FileName);
			return true;
		}
	
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0) return false;
		EditorFile data;
		m_FileHostListbox.GetItemData(m_FileHostListbox.GetSelectedRow(), 0, data);
		
		if (data.FileAtrributes == FileSearchMode.FOLDERS) {
			string filter = "*";
			LoadFileDirectory(data.GetFile(), filter);
			return true;
		}
		
		return true;
	}*/
}


class ExportListItemData
{
	Widget Root;
	string Text;
	string Ext;
	ExportMode Mode;
	
	void ExportListItemData(Widget w, string text, string ext, ExportMode mode)
	{
		Root = w; Text = text; Ext = ext; Mode = mode;
	}
	
	string GetFormattedText()
	{
		return string.Format("%1 (*.%2)", Text, Ext);
	}
	
		
}


class EditorFileExportDialog: EditorFileDialog
{
	protected EditBoxWidget m_FileNameBox;
	
	protected ButtonWidget m_EditorDropdownPrefab;
		
	protected WrapSpacerWidget m_EditorDropdownWraper;
	protected TextListboxWidget m_EditorDropdownListbox;
	protected ScrollWidget m_EditorDropdownScroller;
	protected GridSpacerWidget m_EditorDropdownScrollerContent;
	
	protected ImageWidget m_EditorDropdownExpandIcon;
	protected ImageWidget m_EditorDropdownCollapseIcon;
	protected TextWidget m_EditorDropdownText;
	
	private bool m_DropDownShown;
	private ref array<ref ExportListItemData> m_ExportModes = new array<ref ExportListItemData>();
	private ref ExportListItemData m_SelectedMode;
	
	void EditorFileExportDialog()
	{
		EditorLog.Trace("EditorFileExportDialog");
		
		Widget box_prefab = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorDialogOptionEditText.layout", m_DialogController.ButtonGrid);
		m_FileNameBox = EditBoxWidget.Cast(box_prefab.FindAnyWidget("FileNameEditBox"));
		m_FileNameBox.SetText("Export");
		
		AddButton("Export", "ExportCallback");
		AddButton("Cancel", "Close");
		
		m_EditorDropdownPrefab = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorDialogOptionDropdown.layout", m_DialogController.ButtonGrid));
		m_EditorDropdownWraper = WrapSpacerWidget.Cast(m_EditorDropdownPrefab.FindAnyWidget("EditorDropdownWraper"));
	
		m_EditorDropdownExpandIcon = ImageWidget.Cast(m_EditorDropdownPrefab.FindAnyWidget("EditorDropdownExpandIcon"));
		m_EditorDropdownCollapseIcon = ImageWidget.Cast(m_EditorDropdownPrefab.FindAnyWidget("EditorDropdownCollapseIcon"));
		
		m_EditorDropdownText = TextWidget.Cast(m_EditorDropdownPrefab.FindAnyWidget("EditorDropdownText"));
		
		m_EditorDropdownScroller = ScrollWidget.Cast(m_EditorDropdownPrefab.FindAnyWidget("EditorDropdownScroller"));
		m_EditorDropdownScrollerContent = GridSpacerWidget.Cast(m_EditorDropdownPrefab.FindAnyWidget("EditorDropdownScrollerContent"));
		
		
		m_EditorDropdownScrollerContent.AddChild(CreateDropdownPrefabButton("Server File", "c", ExportMode.COMFILE));
		m_EditorDropdownScrollerContent.AddChild(CreateDropdownPrefabButton("Expansion", "map", ExportMode.EXPANSION));
		m_EditorDropdownScrollerContent.AddChild(CreateDropdownPrefabButton("Vanilla++", "vpp", ExportMode.VPP));
		m_EditorDropdownScrollerContent.AddChild(CreateDropdownPrefabButton("Terrain Builder", "txt", ExportMode.TERRAINBUILDER));
		
	
		m_EditorDropdownText.SetText(m_ExportModes.Get(0).GetFormattedText());
		
		float x, y;
		m_EditorDropdownScrollerContent.GetSize(x, y);
		m_EditorDropdownScroller.SetSize(x, y);
		
		ShowDropdown(false);


		
		string filter = "*";
		LoadFileDirectory(m_StartDirectory, filter);
		
	}
	
	
	void ~EditorFileExportDialog() {
		EditorLog.Trace("~EditorFileExportDialog");
	}
	
	void ExportCallback()
	{
		ExportSettings settings = new ExportSettings();
		settings.ExportFileMode = m_SelectedMode.Mode;
		
		// temp until adding more settings to export window
		if (settings.ExportFileMode == ExportMode.TERRAINBUILDER)
			settings.ExportOffset = Vector(200000, 0, 0);
		
		if (settings.ExportFileMode == ExportMode.VPP)
			settings.ExportSetName = "DayZ Editor Export";
		
		//GetEditor().Export(settings, m_CurrentDirectory + m_FileNameBox.GetText() + "." + m_SelectedMode.Ext);
		Close();
	}
	
	private ButtonWidget CreateDropdownPrefabButton(string text, string ext, ExportMode mode)
	{
		ButtonWidget btn = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorDropdownElement.layout"));
		TextWidget textwidget = TextWidget.Cast(btn.FindAnyWidget("ElementText"));
		ExportListItemData data = new ExportListItemData(btn, text, ext, mode);
		m_ExportModes.Insert(data);
		textwidget.SetText(data.GetFormattedText());
		
		return btn;
	}

	
	private void ShowDropdown(bool show)
	{
		m_EditorDropdownWraper.Show(show);
		m_EditorDropdownExpandIcon.Show(!show);
		m_EditorDropdownCollapseIcon.Show(show);
	}
	
	private ExportListItemData GetExportModeFromPrefab(Widget w)
	{
		foreach (ExportListItemData data: m_ExportModes)
			if (data.Root == w)
				return data;
		
		return null;
	}
	/*
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorFileExportDialog::OnClick");
		
		if (button != 0) return false; 

		
		if (w == m_FileHostListbox) {
			EditorFile data;
			m_FileHostListbox.GetItemData(m_FileHostListbox.GetSelectedRow(), 0, data);
			m_FileNameBox.SetText(data.FileName);
			return true;
		}
		
		if (w == m_EditorDropdownPrefab) {
			m_DropDownShown = !m_DropDownShown;
			ShowDropdown(m_DropDownShown);
			return true;
		}
		
		ExportListItemData mode = GetExportModeFromPrefab(w);
		if (mode != null) {
			m_SelectedMode = mode;
			m_EditorDropdownText.SetText(m_SelectedMode.GetFormattedText());
			
			m_DropDownShown = false;
			ShowDropdown(m_DropDownShown);
			
			return true;
		}
	
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0) return false;
		EditorFile data;
		m_FileHostListbox.GetItemData(m_FileHostListbox.GetSelectedRow(), 0, data);
		
		if (data.FileAtrributes == FileSearchMode.FOLDERS) {
			string filter = "*";
			LoadFileDirectory(data.GetFile(), filter);
			return true;
		}
		
		return true;
	}*/
}