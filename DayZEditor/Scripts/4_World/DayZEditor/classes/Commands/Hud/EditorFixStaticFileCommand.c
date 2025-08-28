[RegisterEditorCommand(EditorFixStaticFileCommand)]
class EditorFixStaticFileCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		auto selected_objects = m_Editor.GetSelectedObjects();		
		foreach (int id, EditorObject selected_object: selected_objects) {			
			if (selected_object.GetType().Contains(".p3d")) {
				EditorAction undo = new EditorAction("SetTransform", "SetTransform");
				undo.InsertUndoParameter(selected_object.GetTransformArray());
				vector center = GetP3dBoundingCenter(selected_object.GetType());
				selected_object.SetPosition(selected_object.GetPosition() + center);
				selected_object.SetOrientation(selected_object.GetOrientation() * Math.RAD2DEG);
				undo.InsertRedoParameter(selected_object.GetTransformArray());
				m_Editor.InsertAction(undo);
			}
		}
		
		//m_Editor.GetEditorHud().ShowFileDialog("Select File to Repair", EditorDZEFile, ScriptCaller.Create(OnFileSelected), eDialogMode.IMPORT, eDialogFlags.ALLOW_DOUBLE_CLICK);
		
		return true;
	}
	
	protected void OnFileSelected(string file)
	{
		string file_name = File.GetName(file);
		string file_output = file;
		file_output.Replace(".dze", "_FIXED.dze");
		
		EditorSaveData save_data = new EditorSaveData();
		
		FileSerializer serializer = new FileSerializer();
		serializer.Open(file, FileMode.READ);
		
		if (!save_data.Read(serializer)) {
			GetEditor().GetEditorHud().CreateNotification("Failed to load file");
			serializer.Close();
			return;
		}
		
		serializer.Close();
		
		foreach (EditorObjectData object_data: save_data.EditorObjects) {
			if (object_data.Type.Contains(".p3d")) {
				vector center = GetP3dBoundingCenter(object_data.Type);
				object_data.Position = object_data.Position + center;
				object_data.Orientation = object_data.Orientation * Math.RAD2DEG;
			}
		}
		
		serializer.Open(file_output, FileMode.WRITE);

		save_data.Write(serializer, EditorSaveData.Version);
		GetEditor().GetEditorHud().CreateNotification(string.Format("Fixed file written to %1", file_output));
		serializer.Close();
	}
	
	override string GetName() 
	{
		return "Repair Static File";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.WRENCH_SIMPLE;
	}
}

[RegisterEditorCommand(EditorUnFixStaticFileCommand)]
class EditorUnFixStaticFileCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		auto selected_objects = m_Editor.GetSelectedObjects();		
		foreach (int id, EditorObject selected_object: selected_objects) {			
			if (selected_object.GetType().Contains(".p3d")) {
				EditorAction undo = new EditorAction("SetTransform", "SetTransform");
				undo.InsertUndoParameter(selected_object.GetTransformArray());
				vector center = GetP3dBoundingCenter(selected_object.GetType());
				selected_object.SetPosition(selected_object.GetPosition() - center);
				selected_object.SetOrientation(selected_object.GetOrientation() * Math.DEG2RAD);
				undo.InsertRedoParameter(selected_object.GetTransformArray());
				m_Editor.InsertAction(undo);
			}
		}
		
		//m_Editor.GetEditorHud().ShowFileDialog("Select File to Repair", EditorDZEFile, ScriptCaller.Create(OnFileSelected), eDialogMode.IMPORT, eDialogFlags.ALLOW_DOUBLE_CLICK);
		
		return true;
	}
	
	protected void OnFileSelected(string file)
	{
		string file_name = File.GetName(file);
		string file_output = file;
		file_output.Replace(".dze", "_FIXED.dze");
		
		EditorSaveData save_data = new EditorSaveData();
		
		FileSerializer serializer = new FileSerializer();
		serializer.Open(file, FileMode.READ);
		
		if (!save_data.Read(serializer)) {
			GetEditor().GetEditorHud().CreateNotification("Failed to load file");
			serializer.Close();
			return;
		}
		
		serializer.Close();
		
		foreach (EditorObjectData object_data: save_data.EditorObjects) {
			if (object_data.Type.Contains(".p3d")) {
				vector center = GetP3dBoundingCenter(object_data.Type);
				object_data.Position = object_data.Position + center;
				object_data.Orientation = object_data.Orientation * Math.RAD2DEG;
			}
		}
		
		serializer.Open(file_output, FileMode.WRITE);

		save_data.Write(serializer, EditorSaveData.Version);
		GetEditor().GetEditorHud().CreateNotification(string.Format("Fixed file written to %1", file_output));
		serializer.Close();
	}
	
	override string GetName() 
	{
		return "Unrepair Static File";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.WRENCH_SIMPLE;
	}
}