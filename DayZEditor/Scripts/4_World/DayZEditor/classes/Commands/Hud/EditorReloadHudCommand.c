class EditorReloadHudCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.ReloadHud();
		return true;
	}

	override string GetName() 
	{
		return "#STR_EDITOR_CMD_RELOAD_UI";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_gui image:icon_refresh";
	}
}