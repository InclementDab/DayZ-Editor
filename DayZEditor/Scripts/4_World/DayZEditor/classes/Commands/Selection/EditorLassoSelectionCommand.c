[RegisterEditorCommand(EditorLassoSelectionCommand)]
class EditorLassoSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().SetSelectionMode(SelectionMode.LASSO);
		return true;
	}
	
	override string GetName()
	{
		return "Lasso Selection";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.LASSO;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_P };
	}

	override bool IsToggled()
	{
		return GetEditor().GetEditorHud().GetSelectionMode() == SelectionMode.LASSO;
	}
}