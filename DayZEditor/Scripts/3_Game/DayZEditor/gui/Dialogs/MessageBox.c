class MessageBox: DialogBase
{
	protected DialogResult m_CurrentResult = DialogResult.None;
	
	void MessageBox(string title, string caption, MessageBoxButtons buttons)
	{		
		AddContent(new MessageBoxPrefab(caption));
		
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

		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
	}
	

	static DialogResult Show(string title, string caption, MessageBoxButtons buttons)
	{
		MessageBox message_box = new MessageBox(title, caption, buttons);		
		return message_box.ShowDialog();
	}
	
	static MessageBox ShowSynchronous(string title, string caption, MessageBoxButtons buttons)
	{
		MessageBox message_box = new MessageBox(title, caption, buttons);
		message_box.GetLayoutRoot().Show(true);
		return message_box;
	}
}