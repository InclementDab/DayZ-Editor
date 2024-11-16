class EditorSaveCommand: EditorExportCommandBase
{			
	override bool GetWarnOnOverwrite()
	{
		return GetEditor().GetSaveFile() == string.Empty;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_SAVE";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:save";
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