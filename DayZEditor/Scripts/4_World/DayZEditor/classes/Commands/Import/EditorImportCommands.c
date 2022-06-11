class EditorImportCommandBase: EditorAsyncCommand
{
	protected ref ImportSettings m_ImportSettings = new ImportSettings();
	
	void EditorImportCommandBase()
	{
		m_ImportSettings.SetFileType(GetFileType());
	}
	
	protected override void Call(Class sender, CommandArgs args)
	{
		string extension = "*" + EditorFileType.Cast(GetFileType().Spawn()).GetExtension();
		EditorLog.Debug("Using filter %1", extension);
		EditorFileDialog file_dialog(GetName(), extension, "", GetDialogButtonName(), m_ImportSettings);
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		if (file_name == string.Empty) {
			MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
			return;
		}
		
		GetEditor().LoadSaveData(ImportFile(file_name));
	}
		
	EditorSaveData ImportFile(string file_name)
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
		
		return file_type.Import(file_name, m_ImportSettings);
	}
		
	typename GetFileType();
	
	string GetDialogButtonName() 
	{
		return "#STR_EDITOR_IMPORT";
	}
	
	ImportSettings GetImportSettings()
	{
		return m_ImportSettings;
	}
}