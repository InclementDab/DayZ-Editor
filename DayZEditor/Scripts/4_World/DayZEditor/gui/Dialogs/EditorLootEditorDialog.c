class EditorLootEditorDialog: EditorDialogBase
{
	void EditorLootEditorDialog(string title)
	{
		AddContent(new MessageBoxPrefab("#STR_EDITOR_LOOT_DESC"));
		
		AddButton(DialogResult.OK);
		AddButton("#STR_EDITOR_WIKI", "OpenWiki");
	}
	
	void OpenWiki()
	{
		GetGame().OpenURL("github.com/InclementDab/DayZ-Editor/wiki/%5B8.1%5D-Manual-Loot-Editing-of-MapGroupProto.xml");
	}
}