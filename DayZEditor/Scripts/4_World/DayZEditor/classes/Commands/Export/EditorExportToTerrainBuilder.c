class EditorExportToTerrainBuilder: EditorExportCommandBase
{	
	override typename GetFileType() 
	{
		return EditorTerrainBuilderFile;
	}
	
	override string GetName() 
	{
		return "Export to Terrain Builder";
	}
}