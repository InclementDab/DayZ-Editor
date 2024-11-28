#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorSaveBackupCommand)]
#endif
class EditorSaveBackupCommand: EditorExportCommandBase
{		
	protected override bool Execute(Class sender, CommandArgs args)
	{
		MakeDirectory(SystemPath.Format(GetEditor().GetSettings().BackupDirectory));
		string file_full_path = GetEditor().GetSaveFile();
		
		if (file_full_path != string.Empty) {
			EditorLog.Info("Using filter %1", "*.dze");
			m_ExportSettings.SetFileType(GetFileType());
			string file_name = File.GetName(file_full_path);
			string file_extension = File.GetExtension(file_full_path);
			file_name.Replace(file_extension, string.Empty);	
			DateTime date = DateTime.Now(false);
			string formatted_date = date.ToString("yyyy-MM-dd-HH-mm-ss");
			file_name = string.Format("%1 (%2)%3", file_name, formatted_date, GetFileTypeInstance().GetExtension());
			file_name = SystemPath.Combine(GetEditor().GetSettings().BackupDirectory, file_name);
			ExportFile(file_name, m_ExportSettings, 0);
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