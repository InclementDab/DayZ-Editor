class EditorImportFromTerrainBuilder: EditorImportCommandBase
{
	override string GetName() 
	{
		return "Import from Terrain Builder";
	}
	
	override typename GetFileType() 
	{
		return EditorTerrainBuilderFile;
	}
}