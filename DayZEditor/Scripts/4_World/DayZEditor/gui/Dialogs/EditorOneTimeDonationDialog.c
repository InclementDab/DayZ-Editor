class RichTextPrefab: ScriptView
{
	TextListboxWidget ListBox;
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\prefabs\\RichTextPrefab.layout";
	}
}

class Payload_Changelog: Managed
{
	ref array<string> changelog = {};
	string uploadDate;
	string updateVersion;
}

class EditorOneTimeDonationDialog: EditorDialogBase
{
	protected ref TextBoxPrefab m_RichTitle, m_RichTitle2;
	protected ref RichTextPrefab m_RichText;
	protected ref MessageBoxPrefab m_TextBox, m_TextBox2;
	protected ref CheckBoxPrefab m_NeverShowAgain;
	protected bool m_NeverShowAnymore;
	
	protected string m_Text;
	
	void EditorOneTimeDonationDialog(string title)
	{
		RestContext ctx = CreateRestApi().GetRestContext("http:\/\/astro.pylex.xyz:10078\/");
		string changelog = ctx.GET_now("api\/changelog");
		JsonSerializer serializer = new JsonSerializer();
		Payload_Changelog cl = new Payload_Changelog();
		string error;
		if (serializer.ReadFromString(cl, changelog, error)) {
			m_RichText = new RichTextPrefab();
			m_RichText.ListBox.AddItem(string.Format("Change Log (%1)", cl.uploadDate), null, 0);
			m_RichText.ListBox.AddItem(string.Format("Version %1", cl.updateVersion), null, 0);
			m_RichText.ListBox.AddItem("", null, 0);
			foreach (string message_line: cl.changelog) {
				m_RichText.ListBox.AddItem(message_line, null, 0);
			}
			
			float s_x, s_y;
			m_RichText.ListBox.GetScreenSize(s_x, s_y);
			m_RichText.ListBox.SetScreenSize(s_x, cl.changelog.Count() * 22);
	
			AddContent(m_RichText);
		}

		m_TextBox = new MessageBoxPrefab("DayZ Editor is 100% free to use—and it always will be! But if you want to see even more awesome updates, consider joining our supporter community on Discord. Your support drives future content and improvements!");
		m_NeverShowAgain = new CheckBoxPrefab("Don't Show Again", this, "m_NeverShowAnymore");
		
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
			GetEditor().GetSettings().Save();
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