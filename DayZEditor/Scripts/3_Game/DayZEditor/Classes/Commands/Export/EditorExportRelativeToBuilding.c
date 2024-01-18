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
		auto objects = EditorObject.GetSelectedEditorObjects();
		foreach (EditorObject editor_object: objects) {
			vector position = editor_object.GetWorldObject().GetPosition().InvMultiply4(mat);
			vector orientation = editor_object.GetWorldObject().GetOrientation() - object.GetOrientation();
			
			vector editor_object_mat[4];
			vector editor_object_mat_output[4];
			editor_object.GetWorldObject().GetTransform(editor_object_mat);
			
			Math3D.MatrixInvMultiply4(mat, editor_object_mat, editor_object_mat_output);			
			save_data.EditorObjects.Insert(EditorObjectData.Create(editor_object.GetWorldObject().GetType(), editor_object_mat_output, EFE_DEFAULT));
		}
		
		EditorInitFile init_file = new EditorInitFile();
		init_file.Export(save_data, Editor.ROOT_DIRECTORY + "relative.c", new ExportSettings());
		
		GetEditor().GetEditorHud().ShowNotification("Positions Saved to relative.c file", COLOR_GREEN);
		
		
		EditorObject.ClearSelections();
	}
	
	override bool CanExecute()
	{
		return EditorObject.GetSelectedEditorObjects().Count() > 0;
	}
	
	override string GetName()
	{
		return "Export Relative to Object";
	}
}