

static void SyncThread()
{
	EditorFileSelectDialog file_select("File Select");
	file_select.ShowDialog();
}


class EditorFileSelectDialog: EditorDialogBase
{
	protected autoptr ListBoxPrefab<EditorFile> m_ListBoxPrefab;
	protected string m_CurrentDirectory;
	
	protected string m_Filter;
	
	void EditorFileSelectDialog(string title)
	{
		m_ListBoxPrefab = new ListBoxPrefab<EditorFile>();		
		AddContent(m_ListBoxPrefab);
		
		m_Filter = "*";
		LoadFileDirectory("$profile:\\", m_Filter);
	}
	
	private void LoadFiles(string directory, string filter, inout ref array<ref EditorFile> folder_array, FileSearchMode search_mode)
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
		EditorLog.Info("EditorFileDialog::Loading Directory %1", m_CurrentDirectory);
		m_ListBoxPrefab.GetListBoxPrefabController().ListBoxData.Clear();
		ref	array<ref EditorFile> editor_file_array = {};
				
		LoadFiles(directory, filter, editor_file_array, FileSearchMode.FOLDERS);
		LoadFiles(directory, filter, editor_file_array, FileSearchMode.FILES);

		foreach (EditorFile sorted_file: editor_file_array) {
			m_ListBoxPrefab.GetListBoxPrefabController().ListBoxData.Insert(sorted_file);
		}
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorFileDialog::OnDoubleClick");
		
		string file;
		m_ListBoxPrefab.ListBox.GetItemText(m_ListBoxPrefab.ListBox.GetSelectedRow(), 0, file);
		
		TStringArray file_check = {};
		file.Split(".", file_check);
		
		LoadFileDirectory(m_CurrentDirectory + "\\" + file, m_Filter);
		
		return true;
	}
}