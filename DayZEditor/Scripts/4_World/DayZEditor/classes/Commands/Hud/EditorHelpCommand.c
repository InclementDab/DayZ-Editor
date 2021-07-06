class EditorHelpCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorHelpDialog help_dialog(GetName());
		help_dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_HELP";
	}
}