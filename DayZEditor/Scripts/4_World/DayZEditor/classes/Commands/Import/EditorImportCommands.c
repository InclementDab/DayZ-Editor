class EditorImportCommandBase: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		string extension = "*" + EditorFileType.Cast(GetFileType().Spawn()).GetExtension();
		EditorLog.Debug("Using filter %1", extension);
		EditorFileDialog file_dialog(GetName(), extension, "", GetDialogButtonName());
		
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
		
		file_name = Editor.ROOT_DIRECTORY + file_name;
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());
		if (!FileExist(file_name)) {
			EditorLog.Error("Could not find file %1", file_name);
			return null;
		}
		
		EditorSaveData save_data = new EditorSaveData();
		ImportSettings settings = new ImportSettings(); // todo
		save_data = file_type.Import(file_name, settings);
		
		if (save_data.MapName != string.Empty && save_data.MapName != GetGame().GetWorldName()) {
			EditorLog.Warning("Different map detected");
			/*if (MessageBox.Show("Different Map Detected", string.Format("Switch map to %1?"), MessageBoxButtons.OKCancel) != DialogResult.OK) {
				return null;
			}
			
			EditorLog.Info("Loading Map %1", save_data.MapName);
			g_Game.ReportProgress(string.Format("Loading Map %1", save_data.MapName));
			
			GetGame().PlayMission(CreateEditorMission(save_data.MapName));
			
			m_Editor = GetEditor();
			*/
		}
		
		if (clear_before) {
			GetEditor().Clear();
		}
				
		EditorLog.Debug("Deleting %1 Objects", save_data.EditorDeletedObjects.Count().ToString());
		if (save_data.EditorDeletedObjects.Count() > 0 && !GetEditor().GetObjectManager().IsWorldCacheLoaded()) {
			EditorLog.Warning("World Cache not loaded, loading to avoid file corruption");
			GetEditor().GetObjectManager().LoadWorldCache();
		}
		
		foreach (EditorDeletedObjectData id: save_data.EditorDeletedObjects) {
			if (!GetEditor().HideMapObject(id)) {
				EditorLog.Warning("Failed to delete building: %1", id.ToString());
			}
		}
		
		EditorLog.Debug("Creating %1 Objects", save_data.EditorObjects.Count().ToString());
		foreach (EditorObjectData data: save_data.EditorObjects) {
			GetEditor().CreateObject(data, false);
		}
		
		GetEditor().GetEditorHud().CreateNotification(string.Format("Loaded %1 objects! (%2 deletions)", save_data.EditorObjects.Count(), save_data.EditorDeletedObjects.Count()), COLOR_GREEN);
		return save_data;
	}
	
	typename GetFileType();
	
	string GetDialogButtonName() 
	{
		return "#STR_EDITOR_IMPORT";
	}
}