class EditorImportFromExpansion: EditorImportCommandBase
{
	override string GetName() 
	{
		return "Import from .map";
	}
	
	override typename GetFileType() 
	{
		return EditorExpansionFile;
	}
}