class EditorCutCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().CreateNotification(string.Format("Cut %1 items to clipboard!", GetEditor().GetSelectedObjects().Count()), COLOR_SALMON);
		EditorClipboard.Cut();
		return true;
	}

	override string GetName() 
	{
		return "#STR_EDITOR_CUT";
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