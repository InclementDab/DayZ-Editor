class EditorTransformModeMenu: EditorMenu
{
	void EditorTransformModeMenu()
	{
		EditorLog.Trace("EditorTransformModeMenu");
		
		AddMenuButton(m_Editor.CommandManager.MagnetCommand);
		AddMenuButton(m_Editor.CommandManager.GroundCommand);
		AddMenuButton(m_Editor.CommandManager.SnapCommand);
		AddMenuButton(m_Editor.CommandManager.CollisionCommand);
	}
}