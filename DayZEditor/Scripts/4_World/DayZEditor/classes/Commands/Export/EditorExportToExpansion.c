class EditorExportToExpansion: EditorExportCommandBase
{	
	override typename GetFileType() 
	{
		return EditorExpansionFile;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_EXPORT_EXPANSION (*.map)";
	}
}