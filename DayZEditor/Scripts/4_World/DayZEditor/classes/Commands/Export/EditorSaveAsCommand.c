class EditorSaveAsCommand: EditorExportCommandBase
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		//super.Execute(sender, args);
		GetEditor().GetEditorHud().ShowFileDialog(GetName(), GetFileType(), ScriptCaller.Create(OnSaveAsFileSelected), eDialogMode.SAVE, eDialogFlags.WARN_ON_OVERWRITE);
		return true;
	}
	
	protected void OnSaveAsFileSelected(string file_name, eDialogExtraSetting extra_settings)
	{
		if (ExportFile(file_name, m_ExportSettings, extra_settings & eDialogExtraSetting.EXPORT_SELECTED_ONLY)) {
			GetEditor().SetSaveFile(file_name);
		}
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_SAVEAS";
	}

	override Symbols GetSymbol()
	{
		return Symbols.FLOPPY_DISK_PEN;
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
		return "#STR_EDITOR_SAVE";
	}
}