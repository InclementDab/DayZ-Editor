class DialogExitButton: DialogButton
{	
	// Buttons store their result on Message Box
	DialogResult ButtonResult;
	
	void DialogExitButton(Widget parent = null, string label = "", string callback = "", DialogResult result = DialogResult.None)
	{
		ButtonResult = result;
	}
}