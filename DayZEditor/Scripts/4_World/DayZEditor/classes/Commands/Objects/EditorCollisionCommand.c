class EditorCollisionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.CollisionMode = !m_Editor.CollisionMode;
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_COLLISION_MODE";
	}

	override Symbols GetSymbol()
	{
		return Symbols.ARROW_DOWN_SMALL_BIG;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_4 };
	}
	
	override LinearColor GetColor()
	{
		return COLOR_PALE_B;
	}
	
	override bool IsToggled()
	{
		return GetEditor().CollisionMode;
	}
}
