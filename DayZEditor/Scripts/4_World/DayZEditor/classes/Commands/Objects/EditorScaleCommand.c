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
	
	override string GetName() 
	{
		return "Scale";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_R };
	}

	override Symbols GetSymbol()
	{
		return Symbols.ARROWS_MAXIMIZE;
	}
}