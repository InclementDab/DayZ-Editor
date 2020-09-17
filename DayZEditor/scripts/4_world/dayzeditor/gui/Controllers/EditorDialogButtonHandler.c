class EditorDialogButtonViewBinding: ViewBinding
{
	protected Widget DialogButtonOutline;
	protected ButtonWidget DialogButton;
		
	void SetLabel(string label) {
		DialogButton.SetText(label);
	}
}