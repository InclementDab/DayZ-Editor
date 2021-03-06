class EditorImportCommandBase: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorFileDialog file_dialog(GetName(), "*" + EditorFileType.Cast(GetFileType().Spawn()).GetExtension(), "", GetDialogButtonName());
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		if (file_name == string.Empty) {
			MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
			return;
		}
					
		ImportFile(file_name);
	}
	
	protected EditorSaveData ImportFile(string file_name, bool clear_before = false)
	{
		EditorFileType file_type = EditorFileType.Cast(GetFileType().Spawn());
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Import");
			return null;
		}
		
		file_name = "$profile:Editor/" + file_name;
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());
		if (!FileExist(file_name)) {
			MessageBox.Show("File Not Found", "Could not find file " + file_name, MessageBoxButtons.OK);
			return null;
		}
		
		EditorSaveData save_data = new EditorSaveData();
		ImportSettings settings = new ImportSettings(); // todo
		save_data = file_type.Import(file_name, settings);
		
		if (clear_before) {
			m_Editor.Clear();
		}
		
		CF_ObjectManager.UnhideAllMapObjects();
				
		EditorLog.Info("Deleting %1 Objects", save_data.EditorDeletedObjects.Count().ToString());
		
		// Depreciated
		foreach (int id: save_data.DeletedObjects) {
			if (!GetEditor().HideMapObject(id)) {
				EditorLog.Warning("Failed to delete building: %1", id.ToString());
				EditorMessageBox.ShowSynchronous("Error", string.Format("There was an error deleting object %1\nTry restarting the game and reloading the file", id), MessageBoxButtons.OK);
			}
		}
		
		foreach (EditorDeletedObjectData deleted_object: save_data.EditorDeletedObjects) {
			if (!GetEditor().HideMapObject(deleted_object)) {
				EditorLog.Warning("Failed to delete building: %1", id.ToString());
				EditorMessageBox.ShowSynchronous("Error", string.Format("There was an error deleting object %1\nTry restarting the game and reloading the file", deleted_object.Name), MessageBoxButtons.OK);
			}
		}
		
		EditorLog.Info("Creating %1 Objects", save_data.EditorObjects.Count().ToString());
		foreach (EditorObjectData data: save_data.EditorObjects) {
			GetEditor().CreateObject(data, false);
		}

		
		m_Editor.GetEditorHud().CreateNotification(string.Format("Loaded %1 objects! (%2 deletions)", save_data.EditorObjects.Count(), save_data.DeletedObjects.Count()), COLOR_GREEN);
		return save_data;
	}
	
	typename GetFileType();
	
	string GetDialogButtonName() {
		return "Import";
	}
}