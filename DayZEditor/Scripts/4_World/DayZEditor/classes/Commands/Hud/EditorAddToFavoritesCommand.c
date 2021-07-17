class EditorAddToFavoritesCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_ADD_TO_FAVORITES";
	}
}