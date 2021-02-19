class EditorSaveCommand: EditorExportCommandBase
{		
	override void Call(Class sender, CommandArgs args)
	{
		EditorLog.Trace("EditorSaveCommand");
		
		if (m_Editor.EditorSaveFile == string.Empty) {
			EditorFileDialog file_dialog(GetName(), "File", "*", GetDialogButtonName());
			string file_name;
			if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
				return;
			}
			
			m_Editor.EditorSaveFile = file_name;
		}
		
		ExportFile(m_Editor.EditorSaveFile, new ExportSettings());
	}
	
	override string GetName() 
	{
		return "Save";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:save";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_S };
	}
	
	override typename GetFileType() 
	{
		return EditorDZEFile;
	}
	
	override string GetDialogButtonName() 
	{
		return "Save";
	}
}