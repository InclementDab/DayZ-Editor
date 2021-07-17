class EditorJoinDiscordCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetGame().OpenURL("discord.com/invite/5g742yH");
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_JOIN_DISCORD";
	}
}