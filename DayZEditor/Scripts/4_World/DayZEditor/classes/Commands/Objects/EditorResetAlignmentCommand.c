class EditorResetAlignmentCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorAction align_undo = new EditorAction("SetTransform", "SetTransform");
		EditorObjectMap editor_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject editor_object: editor_objects) {
			vector transform[3];
			editor_object.GetTransform(transform);
			align_undo.InsertUndoParameter(editor_object.GetTransformArray());
			
			vector local_ori = editor_object.GetWorldObject().GetDirection();
			transform[0] = vector.Up * local_ori;
			transform[1] = vector.Up;
			transform[2] = vector.Up * (local_ori * vector.Up);
			
			editor_object.SetTransform(transform);
			editor_object.Update();
			
			align_undo.InsertRedoParameter(editor_object.GetTransformArray());
		}
		
		m_Editor.InsertAction(align_undo);
		
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_RESET_ALIGNMENT";
	}
}