class EditorTransformModeMenu: EditorMenu
{
	void EditorTransformModeMenu()
	{
		EditorLog.Trace("EditorTransformModeMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorTranslationCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorRotationCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorScaleCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorMagnetCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorGroundCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSnapCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCollisionCommand]);
	}
}