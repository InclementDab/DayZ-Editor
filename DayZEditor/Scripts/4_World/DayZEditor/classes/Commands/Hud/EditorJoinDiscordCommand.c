class EditorJoinDiscordCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		GetGame().OpenURL("discord.com/invite/5g742yH");
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_JOIN_DISCORD";
	}
}