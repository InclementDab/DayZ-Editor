class EditorEditorMenu: EditorMenu
{
	void EditorEditorMenu()
	{
		EditorLog.Trace("EditorEditorMenu");
		
		AddMenuButton(m_Editor.CommandManager.ReloadHudCommand);
		AddMenuButton(m_Editor.CommandManager.ReloadBrushesCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.EnvironmentControlCommand);
		AddMenuButton(m_Editor.CommandManager.PreferencesCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.JoinDiscordCommand);
		AddMenuButton(m_Editor.CommandManager.OpenWikiCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.DonateCommand);
		AddMenuButton(m_Editor.CommandManager.HelpCommand);
	}
}