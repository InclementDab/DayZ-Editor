class EditorSaveCommand: EditorExportCommandBase
{		
	override void Call(Class sender, CommandArgs args)
	{
		EditorLog.Trace("EditorSaveCommand");
		
		bool warn_on_overwrite = false;
		string file_name = m_Editor.GetSaveFile();
		if (file_name == string.Empty) {
			EditorLog.Info("Using filter %1", "*.dze");
			EditorFileDialog file_dialog(GetName(), "*.dze", "", GetDialogButtonName());
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
		return "#STR_EDITOR_SAVE";
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