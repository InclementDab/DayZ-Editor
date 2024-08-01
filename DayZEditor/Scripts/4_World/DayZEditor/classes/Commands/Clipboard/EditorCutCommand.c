class EditorCutCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);

		auto selected_objects = GetEditor().GetSelectedObjects();
		if (selected_objects.Count() > 0) {
			int selected_object_count = EditorClipboard.Cut(selected_objects.GetValueArray());
			GetEditor().GetEditorHud().CreateNotification(string.Format("%1 entities cut to clipboard", selected_object_count), LinearColor.AQUA);
		}

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