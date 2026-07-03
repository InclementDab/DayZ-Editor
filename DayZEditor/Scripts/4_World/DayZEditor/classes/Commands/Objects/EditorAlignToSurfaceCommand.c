class EditorAlignToSurfaceCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorObjectMap editor_objects = m_Editor.GetSelectedObjects();		
		EditorAction align_undo = new EditorAction("SetTransform", "SetTransform");
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

			vector aligned_transform[4];
			EditorSurfacePlacement.GetAlignedTransform(editor_object, surface_normal, aligned_transform);

			align_undo.InsertUndoParameter(editor_object.GetTransformArray());
			editor_object.SetTransform(aligned_transform);
			editor_object.Update();
			has_changes = true;

			align_undo.InsertRedoParameter(editor_object.GetTransformArray());
		}
		
		if (has_changes) {
			m_Editor.InsertAction(align_undo);
		}
		
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_ALIGN_TO_SURFACE";
	}
	
	override string GetIcon()
	{
		return "set:dayz_editor_gui image:magnet";
	}

	override Symbols GetSymbol()
	{
		return Symbols.IMAGE_LANDSCAPE;
	}
}