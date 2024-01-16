class EditorHideCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		if (m_Editor.IsPlacing()) {
			return false;
		}
		
		array<EditorObject> selected_objects = EditorObject.SelectedObjects;
		foreach (EditorObject selected_object: selected_objects) {
			selected_object.Hide(true);
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_HIDE";
	}
}