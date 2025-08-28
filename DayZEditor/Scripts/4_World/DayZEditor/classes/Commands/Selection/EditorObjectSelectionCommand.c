#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorObjectSelectionCommand)]
#endif
class EditorObjectSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().ToggleObjectSelect();
		return true;
	}
	
	override string GetName()
	{
		return "Toggle Objects Are Clickable";
	}
	
	override LinearColor GetColor()
	{
		return LinearColor.BEIGE;
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.HOUSE_BLANK;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_U };
	}

	override bool IsToggled()
	{
		return GetEditor().GetEditorHud().GetObjectSelect();
	}
}