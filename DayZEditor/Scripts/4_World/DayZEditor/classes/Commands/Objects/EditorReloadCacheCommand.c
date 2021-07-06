class EditorReloadCacheCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		GetEditor().GetObjectManager().LoadWorldCache();
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_RELOAD_WORLD_CACHE";
	}
}