class EditorOpenRecentMenu: EditorMenu
{	
	void EditorOpenRecentMenu()
	{
		EditorLog.Trace("EditorOpenRecentMenu");
		
		TStringArray recent_files = GetEditor().GetRecentFiles();
				
		// its backwards
		for (int i = recent_files.Count() - 1; i >= 0; i--) {
			AddMenuItem(new EditorOpenRecentListItem(String(recent_files[i])));
		}
	}
}