class EditorPreferencesMenu: EditorMenu
{
	void EditorPreferencesMenu()
	{
		EditorLog.Trace("EditorPreferencesMenu");
		
		foreach (string category: EditorPreferencesDialog.CATEGORIES) {
			AddMenuItem(new EditorPreferencesListItem(category));
		}
	}
}