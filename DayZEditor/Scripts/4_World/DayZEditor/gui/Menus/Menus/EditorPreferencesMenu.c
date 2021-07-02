class EditorPreferencesMenu: EditorMenu
{
	void EditorPreferencesMenu()
	{
		EditorLog.Trace("EditorPreferencesMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorPreferencesGeneralCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorPreferencesMarkersCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorPreferencesBrushesCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorPreferencesThemesCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorPreferencesAdvancedCommand]);
	}
}