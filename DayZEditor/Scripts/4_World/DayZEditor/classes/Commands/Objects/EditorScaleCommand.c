#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorScaleCommand)]
#endif
class EditorScaleCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		GetEditor().SetMode(eEditorMode.Scale);
		return true;
	}
	
	override bool IsToggled()
	{
		return GetEditor().GetMode() == eEditorMode.Scale;
	}
	
	override string GetName() 
	{
		return "Scale";
	}
	
	override LinearColor GetColor()
	{
		return LinearColor.BLUE;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_NUMPAD3 };
	}

	override Symbols GetSymbol()
	{
		return Symbols.ARROWS_MAXIMIZE;
	}
}