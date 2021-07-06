class EditorHelpDialog: EditorDialogBase
{
	void EditorHelpDialog(string title)
	{
		AddContent(new MessageBoxPrefab("#STR_EDITOR_HELP_DESC"));
		
		AddButton("Wiki", "OpenWiki");
		AddButton("Discord", "OpenDiscord");
		AddButton("Close", DialogResult.OK);
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