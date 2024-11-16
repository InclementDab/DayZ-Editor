class EditorMagnetCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.MagnetMode = !m_Editor.MagnetMode;
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_MAGNET_MODE";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_1 };
	}

	override Symbols GetSymbol()
	{	
		return Symbols.MAGNET;
	}
	
	override LinearColor GetColor()
	{
		return COLOR_CANDY;
	}
}