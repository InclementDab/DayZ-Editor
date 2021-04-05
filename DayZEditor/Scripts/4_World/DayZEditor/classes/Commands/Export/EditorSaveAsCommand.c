class EditorSaveAsCommand: EditorExportCommandBase
{		
	protected override bool ExportFile(string file_name, ExportSettings export_settings)
	{
		if (!super.ExportFile(file_name, export_settings)) {
			return false;
		}
		
		// only export that does this
		m_Editor.SetSaveFile(file_name);
		return true;
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