class EditorExportToTerrainBuilder: EditorExportCommandBase
{	
	override typename GetFileType() 
	{
		return EditorTerrainBuilderFile;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_EXPORT_TB";
	}
}