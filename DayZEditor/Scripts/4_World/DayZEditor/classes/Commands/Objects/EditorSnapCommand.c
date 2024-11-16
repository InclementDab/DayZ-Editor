class EditorSnapCommand: EditorCommand
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.SnappingMode = !m_Editor.SnappingMode;
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_SNAPPING_MODE";
	}

	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_3 };
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.ARROW_DOWN_LEFT_AND_ARROW_UP_RIGHT_TO_CENTER;
	}
	
	override LinearColor GetColor()
	{
		return COLOR_JELLY;
	}
	
	override bool CanExecute()
	{
		return false;
	}
	
	override bool IsToggled()
	{
		return GetEditor().SnappingMode;
	}
}