class EditorCopyPositionCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		GetEditor().GetEditorHud().CreateNotification(string.Format("Copied current position clipboard!", GetEditor().GetSelectedObjects().Count()), COLOR_GREEN);
		GetGame().CopyToClipboard(string.Format("%1, %2", GetGame().GetCurrentCameraPosition().ToString(true), GetGame().GetCurrentCameraDirection().ToString(true)));
	}

	override string GetName() 
	{
		return "Copy Current Position";
	}
		
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_P };
	}
}