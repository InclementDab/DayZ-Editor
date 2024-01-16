class EditorResetAlignmentCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorAction align_undo = new EditorAction("SetTransform", "SetTransform");
		array<EditorObject> editor_objects = EditorObject.SelectedObjects;
		foreach (EditorObject editor_object: editor_objects) {
			vector transform[4];
			editor_object.GetWorldObject().GetTransform(transform);
			align_undo.InsertUndoParameter(editor_object.GetTransformArray());
			
			vector local_ori = editor_object.GetWorldObject().GetDirection();
			transform[0] = vector.Up * local_ori;
			transform[1] = vector.Up;
			transform[2] = vector.Up * (local_ori * vector.Up);
			
			editor_object.GetWorldObject().SetTransform(transform);
			
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