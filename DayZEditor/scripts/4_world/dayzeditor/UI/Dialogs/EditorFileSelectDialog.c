
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
		m_FileHostListbox.AddItem(filename, new EditorFile(filename, directory, fileattr), 0);

		while (FindNextFile(filehandle, filename, fileattr)) {
			m_FileHostListbox.AddItem(filename, new EditorFile(filename, directory, fileattr), 0);
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
		
		LoadFileDirectory(EDITOR_PROFILE_FOLDER, filter);
		
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