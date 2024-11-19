class EditorFileDialog_DEPRECATED: EditorDialogBase
{
	static const string BACK_DIRECTORY_IDENTIFIER = "...";

	protected autoptr EditBoxPrefab m_EditBoxPrefab;
	// strong reference, since ListBox cant strong ref it
	protected autoptr ListBoxPrefab<string> m_ListBoxPrefab;
	protected ref DropdownListPrefab<string> m_FileTypes;
	protected string m_CurrentDirectory;
	protected FileSettingsBase m_FileSettings;
	protected string m_Filter, m_FileTypeString;
	
	void EditorFileDialog_DEPRECATED(string title, string filter = "*", string default_value = "", string button_name = "", FileSettingsBase file_settings = null)
	{
		m_Filter = filter;		
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

		m_CurrentDirectory = Editor.ROOT_DIRECTORY;
		LoadFileDirectory();

		//AddContent(m_FileTypes);
		AddContent(m_EditBoxPrefab);
		AddButton(button_name, DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
			
	DialogResult ShowDialog(out string edit_data)
	{
		// Need to store this variable since EVERYTHING is deleted after ShowDialog finishes
		EditBoxWidget edit_box = m_EditBoxPrefab.ContentText;
		DialogResult result = ShowDialog();				
		edit_data = edit_box.GetText();
		return result;
	}
	
	void LoadFileDirectory()
	{		
		m_ListBoxPrefab.ClearItems();
				
		array<string> files = Directory.EnumerateFiles(m_CurrentDirectory, m_Filter, 0);
		array<string> folders = Directory.EnumerateDirectories(m_CurrentDirectory);

		folders.Sort();
		files.Sort();
		array<string> all_loaded_files = {};
		all_loaded_files.Insert(BACK_DIRECTORY_IDENTIFIER);
		all_loaded_files.InsertAll(folders);
		all_loaded_files.InsertAll(files);

		foreach (string sorted_file: all_loaded_files) {
			string sorted_file_name = File.GetName(sorted_file);
			Symbols icon = Symbols.FILE;
			if (!sorted_file_name.Contains(".")) {
				icon = Symbols.FOLDER;
			} else if (sorted_file_name == BACK_DIRECTORY_IDENTIFIER) {
				icon = Symbols.ARROW_TURN_LEFT_UP;
			}
			
			m_ListBoxPrefab.InsertItem(sorted_file_name, sorted_file, icon.Solid(), GetEditor().GetSettings().HighlightColor);
		}
		
		PrintFormat("Loaded Directory %1, %2 folders, %3 files, filter %4", m_CurrentDirectory, folders.Count(), files.Count(), m_Filter);
	}
	
	void BackDirectory()
	{						
		
	}
	
	void OnListItemClick(string file, Widget w, int x, int y, int button)
	{
		if (file != string.Empty) {
			m_EditBoxPrefab.GetPrefabController().Value = file;
			m_EditBoxPrefab.GetPrefabController().NotifyPropertyChanged("Value");
		}
	}
	
	void OnListItemDoubleClick(string file, Widget w, int x, int y, int button)
	{
		// Is that shit a folder?
		if (file == BACK_DIRECTORY_IDENTIFIER) {
			m_CurrentDirectory = Directory.GetDirectory(m_CurrentDirectory);
			LoadFileDirectory();
			return;
		}
		
		if (!file.Contains(".") || file.LastIndexOf(".") != file.Length() - 3) {
			m_CurrentDirectory = SystemPath.Combine(m_CurrentDirectory, File.GetName(file));
			LoadFileDirectory();
			return;
		}
		
		m_EditBoxPrefab.ContentText.SetText(SystemPath.Join(m_CurrentDirectory, file));
		CloseDialog(DialogResult.OK);
	}
		
	// Abstracterino
	void LoadFile(string file)
	{
		EditBoxWidget edit_box = m_EditBoxPrefab.ContentText;
		edit_box.SetText(file);
		CloseDialog(DialogResult.OK);
	}
	
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "m_FileTypeString": {
				
				break;
			}
		}
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:open";
	}
}