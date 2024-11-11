class EditorSaveBackupCommand: EditorExportCommandBase
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorLog.Trace("EditorSaveCommand");
		
		string file_full_path = m_Editor.GetSaveFile();
		
		if (file_full_path != string.Empty) {
			EditorLog.Info("Using filter %1", "*.dze");
			m_ExportSettings.SetFileType(GetFileType());
			File file = file_full_path;
			string directory = Directory.GetDirectory(file_full_path);
			string file_name = file.GetFileName();
			string file_extension = file.GetExtension();
			file_name.Replace(file_extension, string.Empty);
			Print(file_name);
			array<string> existing_save_files = Directory.EnumerateFiles(directory, string.Format("{0}*", file_name));
			existing_save_files.Debug();
			
			//ExportFile(file_name, m_ExportSettings, false)
		}
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