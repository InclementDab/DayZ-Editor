class EditorReloadCacheCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetObjectManager().LoadWorldCache();
		return true;
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_RELOAD_WORLD_CACHE";
	}
}