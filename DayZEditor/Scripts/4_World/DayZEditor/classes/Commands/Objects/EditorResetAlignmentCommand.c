class EditorResetAlignmentCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorObjectMap editor_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject editor_object: editor_objects) {
			vector transform[3];
			editor_object.GetTransform(transform);
			vector local_ori = editor_object.GetWorldObject().GetDirection();
			transform[0] = vector.Up * local_ori;
			transform[1] = vector.Up;
			transform[2] = vector.Up * (local_ori * vector.Up);
			
			editor_object.SetTransform(transform);
			editor_object.Update();
		}
		
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_RESET_ALIGNMENT";
	}
}