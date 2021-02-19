class EditorJoinDiscordCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		GetGame().OpenURL("discord.com/invite/5g742yH");
	}
	
	override string GetName() 
	{
		return "Join our Discord...";
	}
}