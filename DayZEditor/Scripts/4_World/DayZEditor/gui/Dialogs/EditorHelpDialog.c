class EditorHelpDialog: EditorDialogBase
{
	void EditorHelpDialog(string title)
	{
		AddContent(new MessageBoxPrefab("#STR_EDITOR_HELP_DESC"));
		
		AddButton("#STR_EDITOR_WIKI", "OpenWiki");
		AddButton("#STR_EDITOR_DISCORD", "OpenDiscord");
		AddButton("#STR_EDITOR_CLOSE", DialogResult.OK);
	}
	
	void OpenDiscord()
	{
		GetGame().OpenURL("discord.com/invite/5g742yH");
	}
	
	void OpenWiki()
	{
		GetGame().OpenURL("github.com/InclementDab/DayZ-Editor/wiki");
	}
}