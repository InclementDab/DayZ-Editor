
class EditorFile
{
	string FileName;
	string FileDirectory;
	FileSearchMode FileAtrributes;
	
	void EditorFile(string name, string directory, FileSearchMode attributes)
	{
		FileName = name; FileDirectory = directory; FileAtrributes = attributes;
	}
	
	string GetFile()
	{
		return string.Format("%1\\%2", FileDirectory, FileName);
	}
}


enum FileSearchMode {
	FOLDERS,
	FILES
};

class EditorFileDialog: EditorDialog
{
	
	protected TextListboxWidget m_FileHostListbox;
	
	void EditorFileDialog()
	{
		EditorPrint("EditorFileDialog");
		Widget content = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorFileDialog.layout", null);
		SetContent(content);
		
		m_FileHostListbox = TextListboxWidget.Cast(m_Root.FindAnyWidget("FolderHostListBox"));
	}
	
	private void LoadFiles(string directory, out ref array<ref EditorFile> folder_array, FileSearchMode search_mode)
	{
		TStringArray name_array = new TStringArray();
		string filename;
		FileAttr fileattr;

		FindFileHandle filehandle = FindFile(directory + "*", filename, fileattr, FindFileFlags.ALL);
		
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
		EditorPrint("EditorFileDialog::LoadFileDirectory");
		string filterdir = string.Format("%1%2", directory, filter);
	
		m_FileHostListbox.ClearItems();
		ref array<ref EditorFile> editor_file_array = new array<ref EditorFile>();
		ref array<ref EditorFile> editor_folder_array = new array<ref EditorFile>();
		TStringArray file_array = new TStringArray();
		TStringArray folder_array = new TStringArray();
		
		LoadFiles(directory, editor_folder_array, FileSearchMode.FOLDERS);
		LoadFiles(directory, editor_file_array, FileSearchMode.FILES);
				

		foreach (EditorFile sorted_folder: editor_folder_array) {
			m_FileHostListbox.AddItem(sorted_folder.FileName, sorted_folder, 0);
		}
		
		foreach (EditorFile sorted_file: editor_file_array) {
			m_FileHostListbox.AddItem(sorted_file.FileName, sorted_file, 0);
		}
		
		
	}
}


class EditorFileOpenDialog: EditorFileDialog
{

	protected ButtonWidget m_OpenButton;
	protected ButtonWidget m_CloseButton;
	
	void EditorFileOpenDialog()
	{
		EditorPrint("EditorFileOpenDialog");
		
		
		m_OpenButton = AddButton("Open");
		m_CloseButton = AddButton("Cancel");
		
		//string filter = "*.dze";
		string filter = "*";
		
		LoadFileDirectory("$profile:\\", filter);
		
	}
	
	
	void ~EditorFileOpenDialog()
	{
		EditorPrint("~EditorFileOpenDialog");
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorFileOpenDialog::OnClick");
		
		if (button != 0) return false; 
		
		if (w == m_OpenButton) {
			string file;
			m_FileHostListbox.GetItemText(m_FileHostListbox.GetSelectedRow(), 0, file);
			GetEditor().Open(file);
			CloseDialog();
			return true;
		} 
		
		if (w == m_CloseButton) {
			CloseDialog();
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
			Print("Double clicked folder " + data.GetFile());
			LoadFileDirectory(data.GetFile(), "\\*");
			return true;
		}
		
		Print(data.GetFile());
		GetEditor().Open(data.GetFile());
		CloseDialog();
		
		return true;
	}
}



class EditorFileSaveDialog: EditorFileDialog
{
	
	protected ButtonWidget m_SaveButton;
	protected ButtonWidget m_CancelButton;
	
	void EditorFileSaveDialog()
	{
		
	}
	
}