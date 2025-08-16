[RegisterEditorCommand(EditorGridCommand)]
class EditorGridCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.GridMode = !m_Editor.GridMode;
		return true;
	}
	
	override string GetName() 
	{
		return "Grid Mode";
	}

	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_4 };
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.GRID_ROUND;
	}
	
	override LinearColor GetColor()
	{
		return LinearColor.SKY_BLUE;
	}
	
	override bool CanExecute()
	{
		return true;
	}
	
	override bool IsToggled()
	{
		return GetEditor().GridMode;
	}
}