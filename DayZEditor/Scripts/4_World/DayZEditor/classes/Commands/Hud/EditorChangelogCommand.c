class EditorChangelogCommand: EditorAsyncCommand
{
	override void Call(Class sender, CommandArgs args)
	{	
		EditorOneTimeDonationDialog dialog("Editor Changelog");
		dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "Changelog";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.WRENCH;
	}
}