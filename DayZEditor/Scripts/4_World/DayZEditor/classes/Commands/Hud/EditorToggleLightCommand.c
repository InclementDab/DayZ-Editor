class EditorToggleLightCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		if (GetEditor().GetSelectedObjects().Count() > 0) {
			return false;
		}
		
		GetEditor().GetCamera().ToggleLight();
		
		return true;
	}
	
	override string GetName() 
	{
		return "Toggle Light";
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_LSHIFT, KeyCode.KC_L };
	}
}