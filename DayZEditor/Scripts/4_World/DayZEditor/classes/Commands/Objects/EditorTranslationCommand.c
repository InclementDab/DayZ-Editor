[RegisterEditorCommand(EditorTranslationCommand)]
class EditorTranslationCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		GetEditor().SetMode(eEditorMode.Translation);
		return true;
	}
	
	override string GetName() 
	{
		return "Translate";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_5 };
	}

	override Symbols GetSymbol()
	{
		return Symbols.UP_DOWN_LEFT_RIGHT;
	}
}