class EditorOpenWikiCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		GetGame().OpenURL("github.com/InclementDab/DayZ-Editor/wiki");
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_WIKI";
	}
}