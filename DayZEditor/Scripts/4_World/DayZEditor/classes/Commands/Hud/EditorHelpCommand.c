class EditorHelpCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorHelpDialog help_dialog("Help");
		help_dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "Help";
	}
}