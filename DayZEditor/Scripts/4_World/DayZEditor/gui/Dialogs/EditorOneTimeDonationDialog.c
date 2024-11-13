class EditorOneTimeDonationDialog: EditorDialogBase
{
	protected ref MessageBoxPrefab m_TextBox;
	protected ref CheckBoxPrefab m_NeverShowAgain;
	protected bool m_NeverShowAnymore;
	
	protected string m_Text;
	
	void EditorOneTimeDonationDialog(string title)
	{
		m_TextBox = new MessageBoxPrefab("DayZ Editor is 100% free to use—and it always will be! But if you want to see even more awesome updates, consider joining our supporter community on Discord. Your support drives future content and improvements!");
		m_NeverShowAgain = new CheckBoxPrefab("Never Show Again", this, "m_NeverShowAnymore");
		AddContent(m_TextBox);
		AddContent(m_NeverShowAgain);
		
		AddButton(new DialogButton("Support Dab!", "OnDonateButton"));
		AddButton(new DialogButton("Join Discord", "OnDiscordButton"));
		AddButton(new DialogButton("Close", "OnCloseButton"));
	}
	
	void OnCloseButton()
	{
		if (m_NeverShowAnymore) {
			GetEditor().GetSettings().VersionRequestedNotToSeeDonationDialog = Editor.VersionNumber;
		}
		
		CloseDialog(DialogResult.OK);
	}
	
	void OnDonateButton()
	{
		GetGame().OpenURL("https:\/\/discord.com\/channels\/738181536029081662\/shop");
		CloseDialog(DialogResult.OK);
	}	
	
	void OnDiscordButton()
	{
		GetGame().OpenURL("https:\/\/discord.gg\/dayz-editor");
		CloseDialog(DialogResult.OK);
	}
}