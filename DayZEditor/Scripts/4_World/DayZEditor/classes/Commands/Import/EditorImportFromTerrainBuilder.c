class EditorImportFromTerrainBuilder: EditorImportCommandBase
{
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_IMPORT_TB (*.txt)";
	}
	
	override typename GetFileType() 
	{
		return EditorTerrainBuilderFile;
	}
}