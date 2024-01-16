class EditorSaveCommand: EditorExportCommandBase
{		
	override void Call(Class sender, CommandArgs args)
	{
		EditorLog.Trace("EditorSaveCommand");
		
		bool warn_on_overwrite = false;
		string file_name = m_Editor.GetSaveFile();
		if (file_name == string.Empty) {
			EditorLog.Info("Using filter %1", "*.dze");
			m_ExportSettings.SetFileType(GetFileType());
			EditorFileDialog file_dialog(GetName(), "*.dze", "", GetDialogButtonName(), m_ExportSettings);
			if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
				return;
			}
			
			warn_on_overwrite = true;
		}
		
		if (ExportFile(file_name, m_ExportSettings, warn_on_overwrite)) {
			m_Editor.SetSaveFile(file_name);
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
	
	override string GetDialogButtonName() 
	{
		return "#STR_EDITOR_SAVE";
	}
}