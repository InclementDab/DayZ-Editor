class EditorSaveCommand: EditorExportCommandBase
{		
	override void Call(Class sender, CommandArgs args)
	{
		EditorLog.Trace("EditorSaveCommand");
		
		bool warn_on_overwrite = false;
		string file_name = m_Editor.GetSaveFile();
		if (file_name == string.Empty) {
			EditorFileDialog file_dialog(GetName(), "File", "*", GetDialogButtonName());
			if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
				return;
			}
			
			warn_on_overwrite = true;
		}
		
		if (ExportFile(file_name, new ExportSettings(), warn_on_overwrite)) {
			m_Editor.SetSaveFile(file_name);
		}
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