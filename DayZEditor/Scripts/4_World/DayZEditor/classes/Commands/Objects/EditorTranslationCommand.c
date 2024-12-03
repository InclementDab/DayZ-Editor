#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorTranslationCommand)]
#endif
class EditorTranslationCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		GetEditor().SetMode(eEditorMode.Translation);
		return true;
	}
	
	override bool IsToggled()
	{
		return GetEditor().GetMode() == eEditorMode.Translation;
	}
	
	override string GetName() 
	{
		return "Translate";
	}
	
	override LinearColor GetColor()
	{
		return LinearColor.GREEN;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_NUMPAD1 };
	}

	override Symbols GetSymbol()
	{
		return Symbols.UP_DOWN_LEFT_RIGHT;
	}
}