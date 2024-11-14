class EditorChangelogCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{	
		super.Execute(sender, args);
		GetEditor().ShowChangelog();
		return true;
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