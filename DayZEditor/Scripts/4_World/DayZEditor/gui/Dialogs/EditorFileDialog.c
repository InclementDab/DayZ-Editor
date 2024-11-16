class EditorFileDialog: EditorDialogBase
{
	protected autoptr EditBoxPrefab m_EditBoxPrefab;
	// strong reference, since ListBox cant strong ref it
	protected autoptr ListBoxPrefab<string> m_ListBoxPrefab;
	protected ref DropdownListPrefab<string> m_FileTypes;
	protected string m_CurrentDirectory;
	protected FileSettingsBase m_FileSettings;
	protected string m_Filter, m_FileTypeString;
	protected ref ScriptCaller m_ScriptCallback;
	
	void EditorFileDialog(string title, ScriptCaller script_cb, string filter = "*", string default_value = "", string button_name = "", FileSettingsBase file_settings = null)
	{
		m_Filter = filter;
		Print(m_Filter);		
		m_ScriptCallback = script_cb;
		m_FileSettings = file_settings;
		m_EditBoxPrefab = new EditBoxPrefab("#STR_EDITOR_FILE", m_Controller, default_value);
	 	//m_FileTypes = new DropdownListPrefab<string>("File Type", this, "m_FileTypeString");
		m_ListBoxPrefab = new ListBoxPrefab<string>();
		m_ListBoxPrefab.Event_OnClick.Insert(OnListItemClick);
		m_ListBoxPrefab.Event_OnDoubleClick.Insert(OnListItemDoubleClick);
		
		AddContent(m_ListBoxPrefab);
				
		if (m_FileSettings) {
			array<ref Param2<string, string>> valid_extensions = {};
			m_FileSettings.GetFileType().GetValidExtensions(valid_extensions);
			foreach (Param2<string, string> valid_extension: valid_extensions) {
				//m_FileTypes.Set(string.Format("%1 (%2)", valid_extension.param1, valid_extension.param2), valid_extension.param2);
			}

			array<ref ScriptView> extra_settings = {};
			m_FileSettings.GetFileSettings(extra_settings);
			if (extra_settings && extra_settings.Count() > 0) {
				GroupPrefab settings_group = new GroupPrefab(m_FileSettings.GetSettingsName(), null, string.Empty);
				foreach (ScriptView setting: extra_settings) {
					settings_group.Insert(setting);
				}
				
				AddContent(settings_group);
			}
		}

		LoadDirectory(Editor.ROOT_DIRECTORY);
		
		AddContent(m_EditBoxPrefab);
		AddButton(button_name, DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
			
	void BackDirectory()
	{
		Print(m_CurrentDirectory);
		m_CurrentDirectory = Directory.GetDirectory(m_CurrentDirectory);
		if (!SystemPath.IsPathRooted(m_CurrentDirectory)) {
			return;
		}
		
		LoadDirectory(m_CurrentDirectory);
	}
	
	void LoadDirectory(string directory)
	{
		m_CurrentDirectory = directory;
		
		m_ListBoxPrefab.ClearItems();
		array<string> directory_file = Directory.EnumerateFiles(m_CurrentDirectory, "*", 5);
		directory_file.Debug();
		array<string> directory_directories = Directory.EnumerateDirectories(m_CurrentDirectory);
		array<string> loaded_files = {};		
		
		loaded_files.InsertAll(directory_directories);
		loaded_files.InsertAll(directory_file);
		m_ListBoxPrefab.InsertItem("...", "...");
		for (int i = 0; i < loaded_files.Count(); i++) {
			Directory file = loaded_files[i];
			m_ListBoxPrefab.InsertItem(file.GetFileName(), loaded_files[i]);
		}
	}
	
	void OnListItemClick(string file, Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorFileDialog::OnListItemClick");
		if (file != string.Empty) {
			FileSystem file_sys = file;
			m_EditBoxPrefab.GetPrefabController().Value = file_sys.GetFileName();
			m_EditBoxPrefab.GetPrefabController().NotifyPropertyChanged("Value");
		}
	}
	
	void OnListItemDoubleClick(string file, Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorFileDialog::OnListItemDoubleClick");
		
		bool is_directory;
		if (file.Length() > 3) {
			is_directory = file[file.Length() - 3] != ".";
		}
				
		// Is that shit a file?
		if (file == "...") {
			BackDirectory();
		}
		else if (is_directory) {
			LoadDirectory(file);		
		} else {
			if (m_ScriptCallback) {
				m_ScriptCallback.Invoke(file);
			}	
		}
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:open";
	}
}