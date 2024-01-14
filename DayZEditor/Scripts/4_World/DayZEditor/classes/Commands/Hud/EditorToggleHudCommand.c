class EditorToggleHudCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		GetEditor().SetHudVisibility(!GetEditor().GetHudVisiblity());
		
		return super.Execute(sender, args);
	}
	
	override string GetName() 
	{
		return "Toggle UI";
	}
}