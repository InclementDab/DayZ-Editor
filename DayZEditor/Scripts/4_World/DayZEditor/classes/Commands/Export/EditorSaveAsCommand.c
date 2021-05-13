class EditorSaveAsCommand: EditorExportCommandBase
{		
	override void Call(Class sender, CommandArgs args)
	{
		EditorLog.Trace("EditorSaveAsCommand");
		
		string file_name = m_Editor.GetSaveFile();
		EditorFileDialog file_dialog(GetName(), "*.dze", "", GetDialogButtonName());
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
			
		if (ExportFile(file_name, new ExportSettings(), true)) {
			m_Editor.SetSaveFile(file_name);
		}
	}
	
	override string GetName() 
	{
		return "Save As...";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_S };
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