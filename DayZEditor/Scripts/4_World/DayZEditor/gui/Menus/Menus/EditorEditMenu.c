class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
				
		AddMenuButton(m_Editor.CommandManager.UndoCommand);
		AddMenuButton(m_Editor.CommandManager.RedoCommand);
		AddMenuDivider();
		AddMenuCategory("Edit", EditorClipboardMenu);
		AddMenuCategory("Transform Mode", EditorTransformModeMenu);
		AddMenuDivider();
		
		AddMenuButton(m_Editor.CommandManager.ScriptEditorCommand);
	}
}