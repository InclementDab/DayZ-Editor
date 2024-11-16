class EditorCopyCommand: EditorCommand
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);

		auto selected_objects = GetEditor().GetSelectedObjects();
		if (selected_objects.Count() > 0) {
			int selected_object_count = EditorClipboard.Copy(selected_objects.GetValueArray());
			GetEditor().GetEditorHud().CreateNotification(string.Format("%1 entities copied to clipboard", selected_object_count));
		}
		
		return true;
	}

	override bool CanExecute()
	{
		return GetEditor().GetSelectedObjects().Count() > 0;
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


	override Symbols GetSymbol()
	{
		return Symbols.COPY;
	}
}