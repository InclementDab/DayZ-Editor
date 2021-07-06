class EditorImportFromTerrainBuilder: EditorImportCommandBase
{
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_IMPORT_TB";
	}
	
	override typename GetFileType() 
	{
		return EditorTerrainBuilderFile;
	}
}