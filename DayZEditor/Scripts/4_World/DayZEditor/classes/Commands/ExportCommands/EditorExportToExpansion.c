class EditorExportToExpansion: EditorExportCommandBase
{	
	override typename GetFileType() 
	{
		return EditorExpansionFile;
	}
	
	override string GetName() 
	{
		return "Export to .map";
	}
}