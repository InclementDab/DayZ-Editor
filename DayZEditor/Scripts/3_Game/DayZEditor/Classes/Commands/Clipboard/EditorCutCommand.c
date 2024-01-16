class EditorCutCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().CreateNotification(string.Format("Cut %1 items to clipboard!", EditorObject.SelectedObjects.Count()), COLOR_SALMON);
		EditorClipboard.Cut();
		return true;
	}
	
	override bool CanExecute()
	{
		return EditorObject.SelectedObjects.Count() > 0;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CUT";
	} 
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:cut";
	}
}