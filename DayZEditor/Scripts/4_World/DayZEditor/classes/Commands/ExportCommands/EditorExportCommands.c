class EditorExportCommandBase: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorExportDialog file_dialog(GetName(), "File", "", GetDialogButtonName());
		
		string file_name;
		ExportSettings export_settings = new ExportSettings();
		if (file_dialog.ShowDialog(file_name, export_settings) != DialogResult.OK) {
			return;
		}
				
		if (file_name == string.Empty) {
			MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
			return;
		}

		ExportFile(file_name, export_settings);
	}
	
	protected void ExportFile(string file_name, ExportSettings export_settings)
	{
		EditorFileType file_type = EditorFileType.Cast(GetFileType().Spawn());
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Export");
			return;
		}
		
		// Warn the user if they are exporting with deleted objects
		if (GetEditor().GetObjectManager().GetDeletedObjects().Count() > 0 && !file_type.IsInherited(EditorDZEFile)) {
			if (EditorMessageBox.Show("Export Warning!", "NOTE: Exporting with this format does NOT support Object Deletion! You need to use .dze file format for this (File > Save)", MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
				return;
			}
		}
		
		file_name = "$profile:Editor/" + file_name;
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());
		
		EditorSaveData save_data = EditorSaveData.CreateFromEditor(m_Editor, export_settings.ExportSelectedOnly);
		
		file_type.Export(save_data, file_name, export_settings);		
		
		string message = string.Format("Saved %1 objects! (%2 deletions)", save_data.EditorObjects.Count().ToString(), CF.ObjectManager.GetHiddenMapObjects().Count());
		m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
	}
	
	typename GetFileType();
	
	string GetDialogButtonName() {
		return "Export";
	}
}