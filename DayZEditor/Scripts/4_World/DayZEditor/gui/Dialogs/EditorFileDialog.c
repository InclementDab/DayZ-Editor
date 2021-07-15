class EditorFileDialog: EditorDialogBase
{
	protected autoptr EditBoxPrefab m_EditBoxPrefab;
	// strong reference, since ListBox cant strong ref it
	protected autoptr ListBoxPrefab<string> m_ListBoxPrefab;
	protected string m_CurrentDirectory;
	protected FileSettingsBase m_FileSettings;
	protected string m_Filter;
	
	void EditorFileDialog(string title, string filter = "*", string default_value = "", string button_name = "", FileSettingsBase file_settings = null)
	{
		m_Filter = filter;		
		m_FileSettings = file_settings;
		m_EditBoxPrefab = new EditBoxPrefab("#STR_EDITOR_FILE", m_Controller, default_value);
	 
		m_ListBoxPrefab = new ListBoxPrefab<string>();
		m_ListBoxPrefab.Event_OnClick.Insert(OnListItemClick);
		m_ListBoxPrefab.Event_OnDoubleClick.Insert(OnListItemDoubleClick);
		AddContent(m_ListBoxPrefab);
		
		LoadFileDirectory(Editor.ROOT_DIRECTORY, m_Filter);
		
		if (m_FileSettings) {
			array<ref ScriptView> extra_settings = {};
			m_FileSettings.GetFileSettings(extra_settings);
			if (extra_settings) {
				foreach (ScriptView setting: extra_settings) {
					AddContent(setting);
				}
			}
		}
		
		AddContent(m_EditBoxPrefab);
		AddButton(button_name, DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	private void LoadFiles(string directory, string filter, inout array<string> folder_array, FileSearchMode search_mode)
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
			folder_array.Insert(sorted_name);
		}
	}
		
	DialogResult ShowDialog(out string edit_data)
	{
		// Need to store this variable since EVERYTHING is deleted after ShowDialog finishes
		EditBoxWidget edit_box = m_EditBoxPrefab.ContentText;
		DialogResult result = ShowDialog();
		edit_data = edit_box.GetText();
		return result;
	}
	
	void LoadFileDirectory(string directory, string filter)
	{
		EditorLog.Trace("EditorFileDialog::LoadFileDirectory");
		m_CurrentDirectory = directory;
		string filterdir = string.Format("%1%2", directory, filter);
		EditorLog.Info("EditorFileDialog::Loading Directory %1", m_CurrentDirectory);
		m_ListBoxPrefab.ClearItems();
		array<string> loaded_files = {};
		loaded_files.Insert("...");

		LoadFiles(directory, filter, loaded_files, FileSearchMode.FOLDERS);
		LoadFiles(directory, filter, loaded_files, FileSearchMode.FILES);

		foreach (string sorted_file: loaded_files) {
			m_ListBoxPrefab.InsertItem(sorted_file, sorted_file);
		}
	}
	
	void BackDirectory()
	{				
		TStringArray file_array = {};
		string file_directory = m_CurrentDirectory;
		file_directory.Split("\\", file_array);
		
		if (file_array.Count() == 1) return;
		
		file_directory.Replace(file_array[file_array.Count() - 1] + "\\", "");		
		LoadFileDirectory(file_directory, m_Filter);
	}
	
	void OnListItemClick(string file, Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorFileDialog::OnListItemClick");
		if (file != string.Empty) {
			string folder_sanitized = m_CurrentDirectory;
			folder_sanitized.Replace(Editor.ROOT_DIRECTORY, "");
			m_EditBoxPrefab.GetPrefabController().Value = folder_sanitized + file;
			m_EditBoxPrefab.GetPrefabController().NotifyPropertyChanged("Value");
		}
	}
	
	void OnListItemDoubleClick(string file, Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorFileDialog::OnListItemDoubleClick");
		
		// Is that shit a folder?
		if (file.Contains("\\")) {
			LoadFileDirectory(m_CurrentDirectory + file, m_Filter);
		} else if (file.Contains("...")) {
			BackDirectory();
		} else {
			CloseDialog(DialogResult.OK);
			//LoadFile(file);
		}
	}
		
	// Abstracterino
	void LoadFile(string file)
	{
		EditorLog.Trace("EditorFileDialog::LoadFile");
		EditBoxWidget edit_box = m_EditBoxPrefab.ContentText;
		edit_box.SetText(file);
		CloseDialog(DialogResult.OK);
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:open";
	}
}