class EditorPreferencesMenu: EditorMenu
{
	void EditorPreferencesMenu()
	{
		EditorLog.Trace("EditorPreferencesMenu");
		
		AddMenuItem(new EditorPreferencesListItem("General"));
		AddMenuItem(new EditorPreferencesListItem("Markers"));
		AddMenuItem(new EditorPreferencesListItem("Brushes"));
		AddMenuItem(new EditorPreferencesListItem("Themes"));
		AddMenuItem(new EditorPreferencesListItem("Advanced"));
	}
}