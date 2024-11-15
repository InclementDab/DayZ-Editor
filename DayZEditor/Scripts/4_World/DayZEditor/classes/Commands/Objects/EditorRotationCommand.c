[RegisterEditorCommand(EditorRotationCommand)]
class EditorRotationCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		GetEditor().SetMode(eEditorMode.Rotation);
		return true;
	}
	
	override string GetName() 
	{
		return "Rotation";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_6 };
	}

	override Symbols GetSymbol()
	{
		return Symbols.ROTATE;
	}
}