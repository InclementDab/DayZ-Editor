class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
				
		AddMenuButton(m_Editor.CommandManager[EditorUndoCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorRedoCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorFindAndReplaceCommand]);
		AddMenuDivider();
		AddMenuCategory("#STR_EDITOR_SELECT", EditorSelectMenu, Symbols.OBJECT_GROUP);
		AddMenuCategory("#STR_EDITOR_EDIT", EditorClipboardMenu, Symbols.CLIPBOARD);
		AddMenuCategory("#STR_EDITOR_TRANSFORM", EditorTransformModeMenu, Symbols.RIGHT_LEFT);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorCheckDuplicatesCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorScriptEditorCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOptionsCommand]);
	}
}

class EditorSelectMenu: EditorMenu
{
	void EditorSelectMenu()
	{		
		AddMenuButton(m_Editor.CommandManager[EditorObjectSelectionCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorBoxSelectionCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorEllipseSelectionCommand]);	
		AddMenuButton(m_Editor.CommandManager[EditorLassoSelectionCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorInvertSelectionCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorClearSelectionCommand]);
	}
}