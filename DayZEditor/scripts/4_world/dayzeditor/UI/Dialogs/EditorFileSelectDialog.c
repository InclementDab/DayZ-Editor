


class EditorFileSelectDialog: EditorDialog
{
	
	protected TextListboxWidget m_FileHostListbox;
	
	protected ButtonWidget m_OpenButton;
	protected ButtonWidget m_CloseButton;
	
	void EditorFileSelectDialog(string rootdir)
	{
		EditorPrint("EditorFileSelectDialog");
		Widget content = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorFileDialog.layout", null);
		SetContent(content);
		
		m_FileHostListbox = TextListboxWidget.Cast(m_Root.FindAnyWidget("FolderHostListBox"));
		
		m_OpenButton = AddButton("Open");
		m_CloseButton = AddButton("Cancel");
		
		string filename;
		FileAttr fileattr;
		rootdir = "$profile:Editor/*.dze";
		FindFileHandle filehandle = FindFile(rootdir, filename, fileattr, FindFileFlags.ALL);
		
		m_FileHostListbox.AddItem(filename, null, 0);
		while (FindNextFile(filehandle, filename, fileattr)) {
			Print("Adding File " + filename);
			m_FileHostListbox.AddItem(filename, null, 0);
		}
		
		CloseFindFile(filehandle);
		
	}
	
	
	void ~EditorFileSelectDialog()
	{
		EditorPrint("~EditorFileSelectDialog");
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorFileSelectDialog::OnClick");
		
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
	
	
}