class EditorClipboardMenu: EditorMenu
{
	void EditorClipboardMenu()
	{
		EditorLog.Trace("EditorClipboardMenu");
		
		AddMenuButton(m_Editor.CommandManager.CutCommand);
		AddMenuButton(m_Editor.CommandManager.CopyCommand);
		AddMenuButton(m_Editor.CommandManager.PasteCommand);	
	}
}