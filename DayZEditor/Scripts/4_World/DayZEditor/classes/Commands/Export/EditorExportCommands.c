class EditorExportCommandBase: EditorCommand
{
	protected ref ExportSettings m_ExportSettings = new ExportSettings();
	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_ExportSettings.SetFileType(GetFileType());
		
		EditorFileType file_type = EditorFileType.Cast(GetFileType().Spawn());
		
		EditorHud.CurrentDialog = new EditorFileDialog(GetName(), ScriptCaller.Create(OnDialogCallback), string.Format("*%1", file_type.GetExtension()), "", GetDialogButtonName(), m_ExportSettings);
		EditorHud.CurrentDialog.GetLayoutRoot().Show(true);
		return true;
	}
	
	protected void OnDialogCallback(string file)
	{
		if (file == string.Empty) {
			MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
		}
		
		thread ExportFile(file, m_ExportSettings, GetWarnOnOverwrite());
	}

	protected bool ExportFile(string file_name, ExportSettings export_settings, bool warn_on_overwrite)
	{
		file_name = FileSystem.Format(file_name);
		EditorFileType file_type = EditorFileType.Cast(GetFileType().Spawn());
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Export");
			return false;
		}

		// Warn the user if they are exporting with deleted objects
		if (GetEditor().GetObjectManager().GetDeletedObjects().Count() > 0 && !file_type.CanDoDeletion() ) {
			if (EditorMessageBox.Show("Export Warning!", "NOTE: Exporting with this format does NOT support Object Deletion! You need to use .dze or .map file format for this (File > Save)", MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
				return false;
			}
		}

		export_settings.ExportSetName = file_name;

		file_name = file_name;
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

		File f = file_name;
		string message = string.Format("Saved %1 objects, %2 deletions (%3)", save_data.EditorObjects.Count(), save_data.EditorHiddenObjects.Count(), f.GetFileName());
		m_Editor.GetEditorHud().CreateNotification(message);
		EditorLog.Debug(message);
		m_Editor.SetSaveFile(file_name);
		return true;
	}

	typename GetFileType();

	string GetDialogButtonName()
	{
		return "#STR_EDITOR_EXPORT";
	}
	
	bool GetWarnOnOverwrite()
	{
		return false;
	}

	override Symbols GetSymbol()
	{
		return Symbols.FILE_EXPORT;
	}
}