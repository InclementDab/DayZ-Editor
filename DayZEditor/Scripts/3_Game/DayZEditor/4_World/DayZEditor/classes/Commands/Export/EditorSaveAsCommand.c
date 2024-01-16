class EditorSaveAsCommand: EditorExportCommandBase
{		
	override void Call(Class sender, CommandArgs args)
	{
		EditorLog.Trace("EditorSaveAsCommand");
		
		string file_name = m_Editor.GetSaveFile();
		EditorLog.Info("Using filter %1", "*.dze");
		m_ExportSettings.SetFileType(GetFileType());
		EditorFileDialog file_dialog(GetName(), "*.dze", "", GetDialogButtonName(), m_ExportSettings);
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
			
		if (ExportFile(file_name, m_ExportSettings, true)) {
			m_Editor.SetSaveFile(file_name);
		}
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_SAVEAS";
	}
	
	override typename GetFileType() 
	{
		return EditorDZEFile;
	}
}