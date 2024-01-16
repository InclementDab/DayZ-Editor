class EditorCopyCommand: EditorCommand
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().CreateNotification(string.Format("Copied %1 items to clipboard!", EditorObject.GetSelectedEditorObjects().Count()), COLOR_SALMON);
		EditorClipboard.Copy();
		return true;
	}

	override string GetName() 
	{
		return "#STR_EDITOR_COPY";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:copy";
	}
}