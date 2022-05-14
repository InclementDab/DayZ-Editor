class EditorOpenCommand: EditorImportCommandBase
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorFileDialog file_dialog(GetName(), "*.dze", "", GetDialogButtonName(), m_ImportSettings);
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		GetEditor().LoadSaveData(ImportFile(file_name), true);
		GetEditor().SetSaveFile(file_name);
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