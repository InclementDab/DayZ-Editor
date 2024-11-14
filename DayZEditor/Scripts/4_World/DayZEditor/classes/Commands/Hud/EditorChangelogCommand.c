class EditorChangelogCommand: EditorCommand
{
	protected ref EditorOneTimeDonationDialog m_EditorOneTimeDonationDialog;

	override bool Execute(Class sender, CommandArgs args)
	{	
		super.Execute(sender, args);
		
		GetEditor().ShowDonationDialog();
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