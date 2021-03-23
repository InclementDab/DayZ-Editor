class EditorEditorMenu: EditorMenu
{
	void EditorEditorMenu()
	{
		EditorLog.Trace("EditorEditorMenu");
		
		AddMenuButton(m_Editor.CommandManager.ReloadHudCommand);
		AddMenuButton(m_Editor.CommandManager.ReloadBrushesCommand);
		AddMenuButton(m_Editor.CommandManager.EnvironmentControlCommand);
		AddMenuButton(m_Editor.CommandManager.PreferencesCommand);
		AddMenuButton(m_Editor.CommandManager.JoinDiscordCommand);
		AddMenuButton(m_Editor.CommandManager.OpenWikiCommand);
		AddMenuButton(m_Editor.CommandManager.DonateCommand);
		AddMenuButton(m_Editor.CommandManager.HelpCommand);
	}
}