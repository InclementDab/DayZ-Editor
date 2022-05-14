class EditorOpenCommand: EditorImportCommandBase
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorFileDialog file_dialog(GetName(), "*.dze", "", GetDialogButtonName(), m_ImportSettings);
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		ImportFile(file_name, true);
	}
	
	override EditorSaveData ImportFile(string file_name, bool clear_before = false)
	{
		EditorSaveData save_data = super.ImportFile(file_name, clear_before);
		
		if (save_data) {
			GetEditor().SetSaveFile(file_name);
			GetEditor().GetCamera().SetPosition(save_data.CameraPosition);
		}
		
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