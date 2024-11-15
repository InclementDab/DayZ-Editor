[RegisterEditorCommand(EditorClearSelectionCommand)]
class EditorClearSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().ClearSelection();
		
		return true;
	}
	
	override string GetName()
	{
		return "Clear Selection";
	}
		
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_D };
	}
}