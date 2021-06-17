class EditorCutCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		GetEditor().GetEditorHud().CreateNotification(string.Format("Cut %1 items to clipboard!", GetEditor().GetSelectedObjects().Count()), COLOR_SALMON);
		EditorClipboard.Cut();
	}

	override string GetName() 
	{
		return "Cut";
	} 
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:cut";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_X };
	}
}