class EditorExportCommandBase: EditorAsyncCommand
{
	protected ref ExportSettings m_ExportSettings = new ExportSettings();
	
	protected override void Call(Class sender, CommandArgs args)
	{
		m_ExportSettings.SetFileType(GetFileType());
		EditorFileDialog file_dialog(GetName(), "*", "", GetDialogButtonName(), m_ExportSettings);

		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}

		if (file_name == string.Empty) {
			MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
			return;
		}

		ExportFile(file_name, m_ExportSettings, true);
	}

	protected bool ExportFile(string file_name, ExportSettings export_settings, bool warn_on_overwrite)
	{
		EditorFileType file_type = EditorFileType.Cast(GetFileType().Spawn());
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Export");
			return false;
		}

		// Warn the user if they are exporting with deleted objects
		if (GetEditor().GetDeletedObjects().Count() > 0 && !file_type.CanDoDeletion() ) {
			if (EditorMessageBox.Show("Export Warning!", "NOTE: Exporting with this format does NOT support Object Deletion! You need to use .dze or .map file format for this (File > Save)", MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
				return false;
			}
		}

		export_settings.ExportSetName = file_name;

		file_name = Editor.ROOT_DIRECTORY + file_name;
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

		EditorSaveData save_data = m_Editor.CreateSaveData(export_settings.ExportSelectedOnly);
		file_type.Export(save_data, file_name, export_settings);

		string message = string.Format("Saved %1 objects! (%2 deletions)", save_data.EditorObjects.Count(), save_data.EditorHiddenObjects.Count());
		m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Debug(message);
		return true;
	}

	typename GetFileType();

	string GetDialogButtonName()
	{
		return "#STR_EDITOR_EXPORT";
	}
}