class EditorExportCommandBase : EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		EditorFileDialog file_dialog(GetName(), "*", "", GetDialogButtonName());

		string file_name;
		ExportSettings export_settings = new ExportSettings();
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}

		if (file_name == string.Empty) {
			MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
			return;
		}

		ExportFile(file_name, export_settings, true);
	}

	protected bool ExportFile(string file_name, ExportSettings export_settings, bool warn_on_overwrite)
	{
		EditorFileType file_type = EditorFileType.Cast(GetFileType().Spawn());
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Export");
			return false;
		}

		// Warn the user if they are exporting with deleted objects
		if (GetEditor().GetObjectManager().GetDeletedObjects().Count() > 0 && !file_type.IsInherited(EditorDZEFile)) {
			if (EditorMessageBox.Show("Export Warning!", "NOTE: Exporting with this format does NOT support Object Deletion! You need to use .dze file format for this (File > Save)", MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
				return false;
			}
		}

		export_settings.ExportSetName = file_name;

		file_name = "$profile:Editor/" + file_name;
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());

		if (FileExist(file_name) && warn_on_overwrite) {
			// Easter egg
			string egg;
			if (Math.RandomIntInclusive(0, 100) == 69 || file_name == "PauseChamp.dze") {
				egg = " PauseChamp";
			}

			if (MessageBox.Show("Are you sure?", "File " + file_name + " already exists. Overwrite?" + egg, MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
				return false;
			}
		}

		EditorSaveData save_data = EditorSaveData.CreateFromEditor(m_Editor, export_settings.ExportSelectedOnly);
		file_type.Export(save_data, file_name, export_settings);

		string message = string.Format("Saved %1 objects! (%2 deletions)", save_data.EditorObjects.Count().ToString(), CF.ObjectManager.GetHiddenMapObjects().Count());
		m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
		return true;
	}

	typename GetFileType();

	string GetDialogButtonName()
	{
		return "Export";
	}
}