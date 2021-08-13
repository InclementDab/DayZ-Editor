class EditorImportFromDZE: EditorImportCommandBase
{
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_IMPORT_DZE (*.dze)";
	}
	
	override typename GetFileType() 
	{
		return EditorDZEFile;
	}
}