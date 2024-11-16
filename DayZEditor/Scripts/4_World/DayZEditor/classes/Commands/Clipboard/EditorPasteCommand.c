class EditorPasteCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);

		Raycast cursor_raycast = GetEditor().GetCursorRaycastModeSafe();		
		if (sender.IsInherited(EditorButtonScript)) {
			cursor_raycast = GetEditor().GetCameraRaycast();
		}
		
		if (cursor_raycast && cursor_raycast.Bounce) {
			int paste_count = EditorClipboard.Paste(cursor_raycast.Bounce.Position);
			if (paste_count < 0) {
				GetEditor().GetEditorHud().CreateNotification(string.Format("Paste: exited with error code (%1)", paste_count));
			} else if (paste_count == 1) {
				GetEditor().GetEditorHud().CreateNotification(string.Format("%1 New Entity Pasted", paste_count));
			} else if (paste_count > 1) {
				GetEditor().GetEditorHud().CreateNotification(string.Format("%1 New Entities Pasted", paste_count));
			}
		}

		return true;
	}

	override string GetName() 
	{
		return "#STR_EDITOR_PASTE";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:paste";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_V };
	}

	override Symbols GetSymbol()
	{
		return Symbols.PASTE;
	}
	
	override bool CanExecute()
	{
		// Checking for valid json could be a terrible idea!
		string from_clipboard, error;
		GetGame().CopyFromClipboard(from_clipboard);
		
		return (from_clipboard != string.Empty);
	}
}