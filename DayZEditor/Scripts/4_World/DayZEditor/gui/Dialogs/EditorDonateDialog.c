class EditorDonateDialog: EditorDialogBase
{
	void EditorDonateDialog(string title)
	{
		AddContent(new MessageBoxPrefab("#STR_EDITOR_DONATE_DESC"));
		
		AddButton("Paypal", "OpenPaypal");
		AddButton("Patreon", "OpenPatreon");
		AddButton("Close", DialogResult.OK);
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