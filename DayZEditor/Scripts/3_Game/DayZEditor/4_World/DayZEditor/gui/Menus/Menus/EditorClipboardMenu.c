class EditorClipboardMenu: EditorMenu
{
	void EditorClipboardMenu()
	{
		EditorLog.Trace("EditorClipboardMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorCutCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCopyCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorPasteCommand]);	
	}
}