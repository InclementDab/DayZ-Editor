class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
				
		AddMenuButton(m_Editor.CommandManager[EditorUndoCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorRedoCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorFindAndReplaceCommand]);
		AddMenuDivider();
		AddMenuCategory("Edit", EditorClipboardMenu);
		AddMenuCategory("Transform Mode", EditorTransformModeMenu);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorCheckDuplicatesCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorScriptEditorCommand]);
	}
}