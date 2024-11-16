class EditorOpenCommand: EditorImportCommandBase
{	
	override EditorSaveData OnDialogCallback(string file_name)
	{
		// if (file_name == string.Empty) {
		// 	MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
		// 	return;
		// }

		EditorFileType file_type = EditorFileType.Cast(GetFileType().Spawn());
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Import");
			return null;
		}
		
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());
		if (!FileExist(file_name)) {
			EditorLog.Error("Could not find file %1", file_name);
			return null;
		}
		
		EditorSaveData save_data = file_type.Import(file_name, m_ImportSettings);
		GetEditor().LoadSaveData(save_data, true);
		GetEditor().SetSaveFile(file_name);
		return save_data;
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_OPEN";
	}

	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:open";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_O };
	}
	
	override typename GetFileType() 
	{
		return EditorDZEFile;
	}
	
	override string GetDialogButtonName() 
	{
		return "#STR_EDITOR_OPEN";
	}
}