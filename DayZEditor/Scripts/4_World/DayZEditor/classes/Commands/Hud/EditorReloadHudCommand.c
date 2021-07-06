class EditorReloadHudCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.ReloadHud();
	}

	override string GetName() 
	{
		return "#STR_EDITOR_CMD_RELOAD_UI";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_gui image:icon_refresh";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_U };
	}
}