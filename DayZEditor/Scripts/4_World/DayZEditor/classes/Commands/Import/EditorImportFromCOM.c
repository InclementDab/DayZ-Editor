class EditorImportFromCOM: EditorImportCommandBase
{
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_IMPORT_COM (*.json)";
	}
	
	override typename GetFileType() 
	{
		return EditorCOMFile;
	}
}