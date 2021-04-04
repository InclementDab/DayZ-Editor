class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
				
		AddMenuButton(m_Editor.CommandManager[EditorUndoCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorRedoCommand]);
		AddMenuDivider();
		AddMenuCategory("Edit", EditorClipboardMenu);
		AddMenuCategory("Transform Mode", EditorTransformModeMenu);
		AddMenuDivider();
		
		AddMenuButton(m_Editor.CommandManager[EditorScriptEditorCommand]);
	}
}