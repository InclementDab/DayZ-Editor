class EditorSnapToSurfaceCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		array<EditorObject> editor_objects = m_Editor.GetSelectedObjects();		
		EditorAction align_undo = new EditorAction("SetTransform", "SetTransform");
		foreach (EditorObject editor_object: editor_objects) {
			vector transform[4];
			
			editor_object.GetWorldObject().GetTransform(transform);
			align_undo.InsertUndoParameter(editor_object.GetTransformArray());
			
			// Get Ground Position
			vector ground_position, ground_dir; 
			
			
			vector surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
			vector local_ori = editor_object.GetWorldObject().GetDirection();
			transform[0] = surface_normal * local_ori;
			transform[1] = surface_normal;
			transform[2] = surface_normal * (local_ori * vector.Up);
			transform[3] = ground_position + (surface_normal * editor_object.GetYDistance());
			
			editor_object.GetWorldObject().SetTransform(transform);
			
			align_undo.InsertRedoParameter(editor_object.GetTransformArray());
		}
		
		m_Editor.InsertAction(align_undo);
		
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_SNAP_TO_SURFACE";
	}
	
	override string GetIcon()
	{
		return "set:dayz_editor_gui image:ground";
	}
}