class EditorExportCommandBase: EditorCommand
{
	protected ref ExportSettings m_ExportSettings = new ExportSettings();
	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().ShowFileDialog(GetName(), GetFileType(), ScriptCaller.Create(OnFileSelected), eDialogMode.EXPORT);
		
		return true;
	}
	
	protected void OnFileSelected(string file_name)
	{
		if (!file_name) {
			GetEditor().GetEditorHud().CreateNotification("No file name specified");
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
		if (GetEditor().GetObjectManager().GetDeletedObjects().Count() > 0 && !file_type.CanDoDeletion() ) {
			//if (GetEditor().GetEditorHud().ShowMessageBox("Export Warning!", "NOTE: Exporting with this format does NOT support Object Deletion! You need to use .dze or .map file format for this (File > Save)", MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
			//	return false;
			//}
		}

		export_settings.ExportSetName = file_name;		
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());

		if (FileExist(file_name) && warn_on_overwrite) {
			// Easter egg
			string egg;
			if (Math.RandomIntInclusive(0, 100) == 69 || file_name == "PauseChamp.dze") {
				egg = " PauseChamp";
			}
			
			// todo: overwrite dialog!!!
			//if (MessageBox.Show("Are you sure?", "File " + file_name + " already exists. Overwrite?" + egg, MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
			//	return false;
			//}
		}

		EditorSaveData save_data = GetEditor().CreateSaveData(export_settings.ExportSelectedOnly);
		file_type.Export(save_data, file_name, export_settings);

		string message = string.Format("Saved %1 objects, %2 deletions (%3)", save_data.EditorObjects.Count(), save_data.EditorHiddenObjects.Count(), File.GetName(file_name));
		GetEditor().GetEditorHud().CreateNotification(message);
		EditorLog.Debug(message);
		return true;
	}

	typename GetFileType();

	string GetDialogButtonName()
	{
		return "#STR_EDITOR_EXPORT";
	}

	override Symbols GetSymbol()
	{
		return Symbols.FILE_EXPORT;
	}
}