class EditorSaveAsCommand: EditorExportCommandBase
{			
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