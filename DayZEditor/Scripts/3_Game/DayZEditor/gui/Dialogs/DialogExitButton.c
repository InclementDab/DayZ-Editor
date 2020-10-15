class DialogExitButton: DialogButton
{	
	// Buttons store their result on Message Box
	DialogResult ButtonResult;
	
	void DialogExitButton(string label, string callback, DialogResult result)
	{
		ButtonResult = result;
	}
}