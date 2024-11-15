[RegisterEditorCommand(EditorInvertSelectionCommand)]
class EditorInvertSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorObjectMap objects = GetEditor().GetPlacedObjects();
		foreach (int id, EditorObject editor_object: objects) {
			GetEditor().ToggleSelection(editor_object);
		}
		
		return true;
	}
	
	override string GetName()
	{
		return "Invert Selection";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.REFLECT_HORIZONTAL;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_I };
	}
}