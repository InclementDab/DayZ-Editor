
class EditorFile
{
	string FileName;
	string FileDirectory;
	FileAttr FileAtrributes;
	
	void EditorFile(string name, string directory, FileAttr attributes)
	{
		FileName = name; FileDirectory = directory; FileAtrributes = attributes;
	}
	
	string GetFile()
	{
		return string.Format("%1\\%2", FileDirectory, FileName);
	}
}

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
	
	void LoadFileDirectory(string directory, string filter)
	{
		EditorPrint("EditorFileDialog::LoadFileDirectory");
		string filterdir = string.Format("%1%2", directory, filter);
		m_FileHostListbox.ClearItems();
		string filename;
		FileAttr fileattr;
		FindFileHandle filehandle = FindFile(filterdir, filename, fileattr, FindFileFlags.ALL);
		ref array<ref EditorFile> editor_file_array = new array<ref EditorFile>();
		TStringArray file_array = new TStringArray();
		TStringArray folder_array = new TStringArray();
		
		editor_file_array.Insert(new EditorFile(filename, directory, fileattr));
		if ((fileattr & FileAttr.DIRECTORY) == FileAttr.DIRECTORY) {
			folder_array.Insert(filename);
		} else file_array.Insert(filename);
		

		
		while (FindNextFile(filehandle, filename, fileattr)) {
			editor_file_array.Insert(new EditorFile(filename, directory, fileattr));
			filename.ToLower();
			
			if ((fileattr & FileAttr.DIRECTORY) == FileAttr.DIRECTORY) {
				folder_array.Insert(filename);
			} else file_array.Insert(filename);
		}
		
		
		folder_array.Sort();
		file_array.Sort();
		
		
		ref array<ref EditorFile> sorted_file_array = new array<ref EditorFile>();
		foreach (string sorted_folder_name: folder_array) {
			foreach (EditorFile unsorted_folder: editor_file_array) {
				string lower_name = unsorted_folder.FileName;
				lower_name.ToLower(); 
				if (sorted_folder_name == lower_name) {
					sorted_file_array.Insert(unsorted_folder);
				}
			}
		}
		
		
		
		foreach (string sorted_name: file_array) {
			foreach (EditorFile unsorted_file: editor_file_array) {
				lower_name = unsorted_file.FileName;
				lower_name.ToLower();
				if (sorted_name == lower_name) {
					sorted_file_array.Insert(unsorted_file);
				}
			}
		}
		

		
		
		foreach (EditorFile sorted_file: sorted_file_array) {
			m_FileHostListbox.AddItem(sorted_file.FileName, sorted_file, 0);
		}
		
		CloseFindFile(filehandle);
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
		
		LoadFileDirectory("$profile:", filter);
		
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
		
		if ((data.FileAtrributes & FileAttr.DIRECTORY) == FileAttr.DIRECTORY) {
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