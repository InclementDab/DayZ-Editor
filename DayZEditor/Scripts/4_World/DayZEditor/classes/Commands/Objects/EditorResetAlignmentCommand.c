class EditorResetAlignmentCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorAction align_undo = new EditorAction("SetTransform", "SetTransform");
		EditorObjectMap editor_objects = m_Editor.GetSelectedObjects();
		bool has_changes;
		foreach (EditorObject editor_object: editor_objects) {
			if (!editor_object || editor_object.IsLocked()) {
				continue;
			}

			align_undo.InsertUndoParameter(editor_object.GetTransformArray());
			editor_object.SetOrientation(vector.Zero);
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
		return "#STR_EDITOR_RESET_ALIGNMENT";
	}
}