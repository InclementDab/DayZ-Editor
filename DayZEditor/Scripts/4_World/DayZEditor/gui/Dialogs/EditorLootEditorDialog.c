class EditorLootEditorDialog: EditorDialogBase
{
	void EditorLootEditorDialog(string title)
	{
		AddContent(new MessageBoxPrefab("The loot positions have been copied to your clipboard. You may want to manually change or add USAGE, CONTAINER, CATEGORY, and TAG. This will give you much more control of the loot that spawns. If you need help, please click the Wiki button. There are explanations and listing of all Vanilla values that can be used."));
		
		AddButton(DialogResult.OK);
		AddButton("Wiki", "OpenWiki");
	}
	
	void OpenWiki()
	{
		GetGame().OpenURL("github.com/InclementDab/DayZ-Editor/wiki");
	}
}