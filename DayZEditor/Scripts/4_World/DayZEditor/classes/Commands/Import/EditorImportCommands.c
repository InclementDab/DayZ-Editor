class EditorImportCommandBase: EditorCommand
{
	protected ref ImportSettings m_ImportSettings = new ImportSettings();
	
	void EditorImportCommandBase()
	{
		m_ImportSettings.SetFileType(GetFileType());
	}
	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorFileType file_type = EditorFileType.Cast(GetFileType().Spawn());
		
		EditorHud.CurrentDialog = new EditorFileDialog(GetName(), ScriptCaller.Create(OnDialogCallback), string.Format("*%1", file_type.GetExtension()), "", GetDialogButtonName(), m_ImportSettings);
		EditorHud.CurrentDialog.GetLayoutRoot().Show(true);
		return true;
	}
		
	EditorSaveData OnDialogCallback(string file_name)
	{
		// if (file_name == string.Empty) {
		// 	MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
		// 	return;
		// }

		EditorFileType file_type = EditorFileType.Cast(GetFileType().Spawn());
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Import");
			return null;
		}
		
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());
		if (!FileExist(file_name)) {
			EditorLog.Error("Could not find file %1", file_name);
			return null;
		}
		
		EditorSaveData save_data = file_type.Import(file_name, m_ImportSettings);
		GetEditor().LoadSaveData(save_data);
		return save_data;
	}
		
	typename GetFileType()
	{
		return EditorDZEFile;
	}
	
	string GetDialogButtonName() 
	{
		return "#STR_EDITOR_IMPORT";
	}
	
	ImportSettings GetImportSettings()
	{
		return m_ImportSettings;
	}

	override Symbols GetSymbol()
	{
		return Symbols.FILE_IMPORT;
	}
}