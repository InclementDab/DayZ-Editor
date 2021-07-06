class EditorDonateDialog: EditorDialogBase
{
	void EditorDonateDialog(string title)
	{
		AddContent(new MessageBoxPrefab("#STR_EDITOR_DONATE_DESC"));
		
		AddButton("#STR_EDITOR_PAYPAL", "OpenPaypal");
		AddButton("#STR_EDITOR_PATREON", "OpenPatreon");
		AddButton("#STR_EDITOR_CLOSE", DialogResult.OK);
	}
	
	void OpenPaypal()
	{
		GetGame().OpenURL("paypal.com/donate/?business=YPCJSENU7QZTA&currency_code=USD");
	}
	
	void OpenPatreon()
	{
		GetGame().OpenURL("patreon.com/InclementDab");
	}
}