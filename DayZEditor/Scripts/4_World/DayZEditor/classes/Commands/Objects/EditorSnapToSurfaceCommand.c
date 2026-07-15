class EditorSnapToSurfaceCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorObjectMap editor_objects = m_Editor.GetSelectedObjects();		
		EditorAction snap_undo = new EditorAction("SetTransform", "SetTransform");
		bool has_changes;
		foreach (EditorObject editor_object: editor_objects) {
			if (!editor_object || editor_object.IsLocked()) {
				continue;
			}

			vector surface_position;
			vector surface_normal;
			if (!EditorSurfacePlacement.GetSurfaceBelow(editor_object, surface_position, surface_normal)) {
				continue;
			}

			vector snapped_transform[4];
			if (!EditorSurfacePlacement.GetSnappedTransform(editor_object, surface_position, surface_normal, snapped_transform)) {
				continue;
			}

			snap_undo.InsertUndoParameter(editor_object.GetTransformArray());
			editor_object.SetTransform(snapped_transform);
			editor_object.Update();
			has_changes = true;
			
			snap_undo.InsertRedoParameter(editor_object.GetTransformArray());
		}
		
		if (has_changes) {
			m_Editor.InsertAction(snap_undo);
		}
		
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
	
	override Symbols GetSymbol()
	{
		return Symbols.BLOCK;
	}
}