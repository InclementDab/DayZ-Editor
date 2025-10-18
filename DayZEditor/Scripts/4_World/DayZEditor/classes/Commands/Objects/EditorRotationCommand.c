#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorRotationCommand)]
#endif
class EditorRotationCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		if (GetEditor().GetMode() == eEditorMode.Rotation) {
			GetEditor().SetMode(eEditorMode.None);
		} else {
			GetEditor().SetMode(eEditorMode.Rotation);
		}

		return true;
	}
	
	override bool IsToggled()
	{
		return GetEditor().GetMode() == eEditorMode.Rotation;
	}
	
	override string GetName() 
	{
		return "Rotation Mode";
	}
	
	override LinearColor GetColor()
	{
		return LinearColor.RED;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_NUMPAD2 };
	}

	override Symbols GetSymbol()
	{
		return Symbols.ROTATE;
	}
}