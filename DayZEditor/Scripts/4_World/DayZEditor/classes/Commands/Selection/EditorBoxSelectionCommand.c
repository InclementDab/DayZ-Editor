[RegisterEditorCommand(EditorBoxSelectionCommand)]
class EditorBoxSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().SetSelectionMode(SelectionMode.BOX);
		return true;
	}
	
	override string GetName()
	{
		return "Box Selection";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.SQUARE_DASHED;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_I };
	}
	
	override bool IsToggled()
	{
		return GetEditor().GetEditorHud().GetSelectionMode() == SelectionMode.BOX;
	}
}