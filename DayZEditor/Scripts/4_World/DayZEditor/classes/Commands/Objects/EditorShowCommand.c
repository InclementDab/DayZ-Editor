class EditorShowCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			selected_object.Hide(false);
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_SHOW";
	}
}