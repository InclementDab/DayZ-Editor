class MessageBox: DialogBase
{
	protected DialogResult m_CurrentResult = DialogResult.None;
	
	void MessageBox(Widget parent = null, string title = "", string caption = "", MessageBoxButtons buttons = MessageBoxButtons.OK)
	{
		Debug_Logging = 1;
		
		AddContent(new MessageBoxContent(null, caption));
		
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
	
	
	private void AddButton(DialogResult result)
	{
		MessageBoxButton button = new MessageBoxButton(null, typename.EnumToString(DialogResult, result), "OnButtonPress", result);
		AddButton(button);
	}
	
	private void OnButtonPress(MessageBoxButton button)
	{
		CloseDialog(button.ButtonResult);
	}

	static DialogResult Show(string title, string caption, MessageBoxButtons buttons)
	{
		MessageBox message_box = new MessageBox(null, title, caption, buttons);
		return message_box.ShowDialog();
	}
}

class MessageBoxContent: ScriptView
{
	protected RichTextWidget CaptionText;
	
	void MessageBoxContent(Widget parent = null, string content = "")
	{
		CaptionText.SetText(content);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/MessageBoxContent.layout";
	}
}

enum MessageBoxButtons
{
	OK,
	OKCancel,
	AbortRetryIgnore,
	YesNoCancel,
	YesNo,
	RetryCancel
};

class MessageBoxButton: DialogButton
{	
	// Buttons store their result on Message Box
	DialogResult ButtonResult;
	
	void MessageBoxButton(Widget parent = null, string label = "", string callback = "", DialogResult result = DialogResult.None)
	{
		ButtonResult = result;
	}
}