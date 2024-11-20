class EditorSaveBackupCommand: EditorExportCommandBase
{		
	protected override bool Execute(Class sender, CommandArgs args)
	{
		string file_full_path = GetEditor().GetSaveFile();
		
		if (file_full_path != string.Empty) {
			EditorLog.Info("Using filter %1", "*.dze");
			m_ExportSettings.SetFileType(GetFileType());
			File file = file_full_path;
			string directory = Directory.GetDirectory(file_full_path);
			string file_name = file.GetFileName();
			string file_extension = file.GetExtension();
			file_name.Replace(file_extension, string.Empty);	
			DateTime date = DateTime.Now(false);
			string formatted_date = date.ToString("yyyy-MM-dd-HH-mm-ss");
			file_name = string.Format("%1 (%2)%3", file_name, formatted_date, m_ExportSettings.GetFileType().GetExtension());			
			ExportFile(file_name, m_ExportSettings, false);
		}
						
		return true;
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_SAVE";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:save";
	}
	
	override typename GetFileType() 
	{
		return EditorDZEFile;
	}
}