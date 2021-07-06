class EditorDonateCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{	
		EditorDonateDialog donate_dialog("Donate");
		donate_dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_DONATE";
	}
}