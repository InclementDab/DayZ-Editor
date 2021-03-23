class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
				
		AddMenuButton(m_Editor.CommandManager.UndoCommand);
		AddMenuButton(m_Editor.CommandManager.RedoCommand);
		AddMenuCategory("Edit", EditorClipboardMenu);
		AddMenuCategory("Transform Mode", EditorTransformModeMenu);
		
		AddMenuButton(m_Editor.CommandManager.ScriptEditorCommand);
	}
}