class EditorBrushToggleCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		return true;
	}	
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_TOGGLE_BRUSH";
	}

	override Symbols GetSymbol()
	{
		return Symbols.BRUSH;
	}
}