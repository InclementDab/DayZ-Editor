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

class ChangelogRestCallback: RestCallbackBase
{
	protected ref ScriptCaller m_Callback;
	
	void ChangelogRestCallback(ScriptCaller callback)
	{
		m_Callback = callback;
	}

	override void OnSuccess(string data, int dataSize)
	{
		WebApiLog.Trace(this, "OnSuccess");
		WebApiLog.Log(string.Format("%1 returned success from api, completed in %2ms", Type(), GetGame().GetTime() - m_CallTime));

		JsonSerializer serializer = new JsonSerializer();
		Payload_Changelog payload = new Payload_Changelog();
		string error;
		if (serializer.ReadFromString(payload, data, error) && m_Callback) {
			m_Callback.Invoke(payload);
		}
	}
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
		EditorWebApi web_api = GetEditor().GetWebApi();
		if (web_api && web_api.GetRestContext()) {
			m_RichText = new RichTextPrefab();
			web_api.GetRestContext().GET(new ChangelogRestCallback(ScriptCaller.Create(SetListBoxInfo)), "api\/changelog\/latest");
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
	
	protected void SetListBoxInfo(Payload_Changelog pl)
	{
		if (!pl) {
			return;
		}
		
		m_RichText.ListBox.AddItem(string.Format("Change Log (%1)", pl.uploadDate), null, 0);
		m_RichText.ListBox.AddItem(string.Format("Version %1", pl.updateVersion), null, 0);
		m_RichText.ListBox.AddItem("", null, 0);
		foreach (string message_line: pl.changelog) {
			m_RichText.ListBox.AddItem(message_line, null, 0);
		}
		
		float s_x, s_y;
		m_RichText.ListBox.GetScreenSize(s_x, s_y);
		m_RichText.ListBox.SetScreenSize(s_x, pl.changelog.Count() * 22);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetupDialog, 10);
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