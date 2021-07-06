class EditorImportFromCOM: EditorImportCommandBase
{
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_IMPORT_COM";
	}
	
	override typename GetFileType() 
	{
		return EditorCOMFile;
	}
}