class EditorImportFromExpansion: EditorImportCommandBase
{
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_IMPORT_EXPANSION";
	}
	
	override typename GetFileType() 
	{
		return EditorExpansionFile;
	}
}