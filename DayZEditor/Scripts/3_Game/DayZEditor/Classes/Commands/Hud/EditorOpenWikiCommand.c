class EditorOpenWikiCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetGame().OpenURL("github.com/InclementDab/DayZ-Editor/wiki");
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_WIKI";
	}
}