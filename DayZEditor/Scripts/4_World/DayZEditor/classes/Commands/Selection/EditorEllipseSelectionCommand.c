[RegisterEditorCommand(EditorEllipseSelectionCommand)]
class EditorEllipseSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().SetSelectionMode(SelectionMode.ELLIPSE);
		return true;
	}
	
	override string GetName()
	{
		return "Circle Selection";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.CIRCLE_DASHED;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_O };
	}

	override bool IsToggled()
	{
		return GetEditor().GetEditorHud().GetSelectionMode() == SelectionMode.ELLIPSE;
	}
}