class EditorPreferencesMenu: EditorMenu
{
	void EditorPreferencesMenu()
	{
		EditorLog.Trace("EditorPreferencesMenu");
		
		foreach (string category: EditorPreferencesDialog.CATEGORIES) {
			// lol, lmao even
			if (category == "#STR_EDITOR_CAMERA" && !GetEditor().GetCamera().GetSettings().LegacyCamera) {
				continue;
			}
			
			AddMenuItem(new EditorPreferencesListItem(category));
		}
	}
}