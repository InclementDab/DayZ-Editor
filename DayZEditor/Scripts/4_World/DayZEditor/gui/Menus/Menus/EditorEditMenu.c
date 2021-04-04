class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
				
		AddMenuButton(m_Editor.CommandManager[EditorUndoCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorRedoCommand]);
		AddMenuCategory("Edit", EditorClipboardMenu);
		AddMenuCategory("Transform Mode", EditorTransformModeMenu);
		
		AddMenuButton(m_Editor.CommandManager[EditorScriptEditorCommand]);
	}
}