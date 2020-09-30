class MessageBox: DialogBase
{
	protected DialogResult m_CurrentResult = DialogResult.None;
	
	void MessageBox(Widget parent = null, string title = "", string caption = "", MessageBoxButtons buttons = MessageBoxButtons.OK)
	{
		Debug_Logging = 1;
		
		AddContent(new MessageBoxPrefab(null, caption));
		
		switch (buttons) {
			
			case MessageBoxButtons.OK: {
				AddButton(DialogResult.OK);
				break;
			}
			
			case MessageBoxButtons.OKCancel: {
				AddButton(DialogResult.OK);
				AddButton(DialogResult.Cancel);
				break;
			}
			
			case MessageBoxButtons.AbortRetryIgnore: {
				AddButton(DialogResult.Abort);
				AddButton(DialogResult.Retry);
				AddButton(DialogResult.Ignore);
				break;
			}
			
			case MessageBoxButtons.YesNoCancel: {
				AddButton(DialogResult.Yes);
				AddButton(DialogResult.No);
				AddButton(DialogResult.Cancel);
				break;
			}
			
			case MessageBoxButtons.YesNo: {
				AddButton(DialogResult.Yes);
				AddButton(DialogResult.No);
				break;
			}			
			
			case MessageBoxButtons.RetryCancel: {
				AddButton(DialogResult.Retry);
				AddButton(DialogResult.Cancel);
				break;
			}
		}
	}
	

	static DialogResult Show(string title, string caption, MessageBoxButtons buttons)
	{
		MessageBox message_box = new MessageBox(null, title, caption, buttons);
		return message_box.ShowDialog();
	}
}