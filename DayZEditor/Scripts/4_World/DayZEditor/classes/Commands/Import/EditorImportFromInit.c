class EditorImportFromInit: EditorImportCommandBase
{
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_IMPORT_INIT_C";
	}
	
	override typename GetFileType() 
	{
		return EditorInitFile;
	}
}