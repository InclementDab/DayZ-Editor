class EditorSaveCommand: EditorExportCommandBase
{		
	protected override bool Execute(Class sender, CommandArgs args)
	{
		//super.Execute(sender, args);
		string file_name = GetEditor().GetSaveFile();
		if (file_name == string.Empty) {			
			GetEditor().GetEditorHud().ShowFileDialog(GetName(), GetFileType(), ScriptCaller.Create(OnSaveFileSelected), eDialogMode.SAVE, eDialogFlags.WARN_ON_OVERWRITE);
		} else {
			OnSaveFileSelected(file_name, 0);
		}
				
		return true;
	}
	
	protected void OnSaveFileSelected(string file_name, eDialogExtraSetting extra_setting)
	{
		if (!file_name) {
			GetEditor().GetEditorHud().CreateNotification("No file name specified");
			return;
		}

		if (ExportFile(file_name, m_ExportSettings, extra_setting)) {
			GetEditor().SetSaveFile(file_name);
		}
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.FLOPPY_DISK;
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
		return "#STR_EDITOR_SAVE";
	}
}