class Payload_Changelog: Managed
{
	int Id;
	ref array<string> ChangelogContent = {};
	string UploadDate;
	string UpdateVersion;
	string CreatedAt;
}

class ChangelogRestCallback: RestCallbackBase
{
	protected ref ScriptCaller m_Callback;
	protected ref ScriptCaller m_ErrorCallback;
	
	void ChangelogRestCallback(ScriptCaller callback, ScriptCaller error_callback = null)
	{
		m_Callback = callback;
		m_ErrorCallback = error_callback;
	}

	override void OnSuccess(string data, int dataSize)
	{
		WebApiLog.Trace(this, "OnSuccess");
		WebApiLog.Log(string.Format("%1 returned success from api, completed in %2ms", Type(), GetGame().GetTime() - m_CallTime));

		JsonSerializer serializer = new JsonSerializer();
		array<ref Payload_Changelog> payload = {};
		string error;
		if (serializer.ReadFromString(payload, data, error) && m_Callback) {
			m_Callback.Invoke(payload);
		} else if (m_ErrorCallback) {
			Error(error);
			m_ErrorCallback.Invoke();
		}
	}

	override void OnTimeout()
	{
		super.OnTimeout();

		if (m_ErrorCallback) {
			m_ErrorCallback.Invoke();
		}
	}

	override void OnError(int errorCode)
	{
		super.OnError(errorCode);

		if (m_ErrorCallback) {
			m_ErrorCallback.Invoke();
		}
	}
}

class EditorOneTimeDonationDialog: EditorDialogBase
{
	protected TextListboxWidget ListBox;
	protected ButtonWidget PrevButton, NextButton;
	protected TextWidget PageIndicatorText;
	protected CheckBoxWidget NeverShowAgainCheckBox;
	protected ref array<ref Payload_Changelog> m_Changelogs = {};
	protected int m_CurrentChangelogIndex;

	protected string m_Text;

	void EditorOneTimeDonationDialog(string title)
	{
		EditorWebApi web_api = GetEditor().GetWebApi();
		if (web_api && web_api.GetRestContext()) {
			web_api.GetRestContext().GET(new ChangelogRestCallback(ScriptCaller.Create(SetListBoxInfo), ScriptCaller.Create(OnChangelogLoadFailed)), "api\/changelog");
		} else {
			OnChangelogLoadFailed();
		}

		UpdateNavigationButtons();
	}

	protected void SetListBoxInfo(array<ref Payload_Changelog> payload)
	{
		if (!payload || payload.Count() == 0) {
			RenderListMessage("No changelog entries available.");
			return;
		}

		m_Changelogs = payload;
		SortChangelogsById();
		m_CurrentChangelogIndex = 0;
		RenderCurrentChangelog();
	}

	protected void SortChangelogsById()
	{
		if (!m_Changelogs) {
			return;
		}

		for (int i = 0; i < m_Changelogs.Count() - 1; i++) {
			int max_index = i;
			for (int j = i + 1; j < m_Changelogs.Count(); j++) {
				if (m_Changelogs[j] && (!m_Changelogs[max_index] || m_Changelogs[j].Id > m_Changelogs[max_index].Id)) {
					max_index = j;
				}
			}

			if (max_index != i) {
				Payload_Changelog changelog = m_Changelogs[i];
				m_Changelogs[i] = m_Changelogs[max_index];
				m_Changelogs[max_index] = changelog;
			}
		}
	}

	protected void RenderCurrentChangelog()
	{
		if (!m_Changelogs || m_Changelogs.Count() == 0) {
			RenderListMessage("No changelog entries available.");
			return;
		}

		if (m_CurrentChangelogIndex < 0) {
			m_CurrentChangelogIndex = 0;
		}

		if (m_CurrentChangelogIndex >= m_Changelogs.Count()) {
			m_CurrentChangelogIndex = m_Changelogs.Count() - 1;
		}

		Payload_Changelog pl = m_Changelogs[m_CurrentChangelogIndex];
		if (!pl) {
			RenderListMessage("Unable to load changelog.");
			return;
		}

		if (!ListBox) {
			return;
		}
		
		ListBox.ClearItems();
		ListBox.AddItem(string.Format("Change Log (%1)", pl.UploadDate), null, 0);
		ListBox.AddItem(string.Format("Version %1", pl.UpdateVersion), null, 0);
		ListBox.AddItem("", null, 0);

		foreach (string message_line: pl.ChangelogContent) {
			ListBox.AddItem(message_line, null, 0);
		}

		UpdateNavigationButtons();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetupDialog, 10);
	}

	protected void RenderListMessage(string message)
	{
		if (!ListBox) {
			return;
		}

		ListBox.ClearItems();
		ListBox.AddItem(message, null, 0);
		UpdateNavigationButtons();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetupDialog, 10);
	}

	protected void UpdateNavigationButtons()
	{
		int changelog_count = 0;
		if (m_Changelogs) {
			changelog_count = m_Changelogs.Count();
		}

		int current_page = m_CurrentChangelogIndex + 1;

		if (PageIndicatorText) {
			if (changelog_count > 0) {
				PageIndicatorText.SetText(string.Format("%1 / %2", current_page, changelog_count));
			} else {
				PageIndicatorText.SetText("- / -");
			}
		}

		if (PrevButton) {
			PrevButton.Enable(changelog_count > 1 && current_page > 1);
		}

		if (NextButton) {
			NextButton.Enable(changelog_count > 1 && current_page < changelog_count);
		}
	}

	protected void OnChangelogLoadFailed()
	{
		RenderListMessage("Unable to load changelog.");
	}

	void OnPrevButton()
	{
		if (!m_Changelogs || m_CurrentChangelogIndex <= 0) {
			UpdateNavigationButtons();
			return;
		}

		m_CurrentChangelogIndex--;
		RenderCurrentChangelog();
	}

	void OnNextButton()
	{
		if (!m_Changelogs || m_CurrentChangelogIndex >= m_Changelogs.Count() - 1) {
			UpdateNavigationButtons();
			return;
		}

		m_CurrentChangelogIndex++;
		RenderCurrentChangelog();
	}
	
	void OnCloseButton()
	{
		if (NeverShowAgainCheckBox && NeverShowAgainCheckBox.IsChecked()) {
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

	override string GetLayoutFile()
	{
		return "DayZEditor/gui/Layouts/dialogs/EditorChangelogDialog.layout";
	}
}