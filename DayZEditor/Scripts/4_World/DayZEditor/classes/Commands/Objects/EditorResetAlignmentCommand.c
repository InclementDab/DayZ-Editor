class EditorResetAlignmentCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorObjectMap editor_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject editor_object: editor_objects) {
			editor_object.SetOrientation(vector.Zero);
			editor_object.Update();
		}
		
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_RESET_ALIGNMENT";
	}
}