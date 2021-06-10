class EditorDonateDialog: EditorDialogBase
{
	void EditorDonateDialog(string title)
	{
		AddContent(new MessageBoxPrefab("Want to support the DayZ Editor project?\nDonations are much appreciated. But never required! <3"));
		
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