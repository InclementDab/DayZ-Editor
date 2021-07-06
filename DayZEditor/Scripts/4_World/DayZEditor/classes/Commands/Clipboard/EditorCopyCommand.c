class EditorCopyCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		GetEditor().GetEditorHud().CreateNotification(string.Format("Copied %1 items to clipboard!", GetEditor().GetSelectedObjects().Count()), COLOR_SALMON);
		EditorClipboard.Copy();
	}

	override string GetName() 
	{
		return "#STR_EDITOR_COPY";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:copy";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_C };
	}
}