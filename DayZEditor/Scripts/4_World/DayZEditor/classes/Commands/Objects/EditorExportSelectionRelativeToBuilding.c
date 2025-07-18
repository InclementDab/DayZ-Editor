[RegisterEditorCommand(EditorExportSelectionRelativeToBuilding)]
class EditorExportSelectionRelativeToBuilding: EditorCommand
{
	protected Object m_SourceObject;

	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		Param1<EditorObject> data = Param1<EditorObject>.Cast(GetData());
		if (!data) {
			return false;
		}

		m_SourceObject = data.GetWorldObject();
		
		auto selected_objects = GetEditor().GetSelectedObjects().GetValueArray();
		if (selected_objects.Count() == 0) {
			GetEditor().GetEditorHud().CreateNotification("No objects selected to export!");
			return false;
		}
		
		GetEditor().GetEditorHud().ShowFileDialog("Export Relative To Building", EditorDZEFile, ScriptCaller.Create(OnFileSelected), eDialogMode.EXPORT, eDialogFlags.WARN_ON_OVERWRITE, m_SourceObject.GetType());
		return true;
	}
	
	protected void OnFileSelected(string file_name, eDialogExtraSetting extra_settings)
	{
		EditorFileType file_type = new EditorDZEFile();
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());

		vector mat[4];
		m_SourceObject.GetTransform(mat);

		EditorSaveData save_data = GetEditor().CreateSaveData(true);
		for (int i = 0; i < save_data.EditorObjects.Count(); i++) {
			vector mat_object[4];
			Math3D.YawPitchRollMatrix(save_data.EditorObjects[i].Orientation, mat_object);
			mat_object[3] = save_data.EditorObjects[i].Position;

			Math3D.MatrixInvMultiply4(mat_object, mat, mat_object);

			save_data.EditorObjects[i].Orientation = Math3D.MatrixToAngles(mat_object);
			save_data.EditorObjects[i].Position = mat_object[3];
		}

		save_data.EditorHiddenObjects.Clear();
		
		file_type.Export(save_data, file_name, new ExportSettings(), extra_settings);

		string message = string.Format("Saved %1 objects (%2)", save_data.EditorObjects.Count(), File.GetName(file_name));
		GetEditor().GetEditorHud().CreateNotification(message);
	}

	protected override void Update(float dt)
	{
		SetCanExecute(GetEditor().GetSelectedObjects().Count() > 0);
	}

	override string GetName()
	{
		return "Export Relative to Selection";
	}
}
