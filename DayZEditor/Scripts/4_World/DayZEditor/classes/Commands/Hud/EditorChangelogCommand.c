class EditorChangelogCommand: EditorCommand
{
	override void Execute(Class sender, CommandArgs args)
	{	
		super.Execute(sender, args);
		GetEditor().ShowChangelog();
	}
	
	override string GetName() 
	{
		return "Changelog";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.WRENCH_SIMPLE;
	}
}