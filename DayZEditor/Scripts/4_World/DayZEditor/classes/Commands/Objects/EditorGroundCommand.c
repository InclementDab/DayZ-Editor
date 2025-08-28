class EditorGroundCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.GroundMode = !m_Editor.GroundMode;
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_GROUND_MODE";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_2 };
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.ARROW_DOWN_TO_LINE;
	}
	
	override LinearColor GetColor()
	{
		return COLOR_APPLE;
	}
	
	override bool IsToggled()
	{
		return GetEditor().GroundMode;
	}
}