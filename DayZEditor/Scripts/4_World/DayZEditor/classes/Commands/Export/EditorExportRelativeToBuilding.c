class EditorExportRelativeToObject: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args) 
	{
		super.Execute(sender, args);
		
		GetEditor().PromptForObjectSelection(ScriptCaller.Create(OnObjectSelected));
		return true;
	}
	
	void OnObjectSelected(Object object)
	{
		if (!object) {
			return;
		}
		
		vector mat[4];
		object.GetTransform(mat);
		
		EditorSaveData save_data = new EditorSaveData();
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (int id, EditorObject editor_object: selected_objects) {
			vector position = editor_object.GetPosition().InvMultiply4(mat);
			vector orientation = editor_object.Orientation - object.GetOrientation();
			
			vector editor_object_mat[4];
			vector editor_object_mat_output[4];
			editor_object.GetTransform(editor_object_mat);
			
			Math3D.MatrixInvMultiply4(mat, editor_object_mat, editor_object_mat_output);			
			save_data.EditorObjects.Insert(EditorObjectData.Create(editor_object.GetType(), editor_object_mat_output, EditorObjectFlags.ALL));
		}
		
		EditorInitFile init_file = new EditorInitFile();
		init_file.Export(save_data, Editor.ROOT_DIRECTORY + "relative.c", new ExportSettings());
		
		GetEditor().GetEditorHud().CreateNotification("Positions Saved to relative.c file", COLOR_GREEN);
		
		
		m_Editor.ClearSelection();
	}
	
	override bool CanExecute()
	{
		return GetEditor().GetSelectedObjects().Count() > 0;
	}
	
	override string GetName()
	{
		return "Export Relative to Object";
	}
}