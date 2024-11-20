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
		GetEditor().GetEditorHud().ShowFileDialog(GetName(), GetFileType(), ScriptCaller.Create(OnFileSelected), eDialogMode.IMPORT);
		
		return true;
	}
	
	protected void OnFileSelected(string file_name)
	{
		if (!file_name) {
			GetEditor().GetEditorHud().CreateNotification("No file name specified");
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

	override Symbols GetSymbol()
	{
		return Symbols.FILE_IMPORT;
	}
}