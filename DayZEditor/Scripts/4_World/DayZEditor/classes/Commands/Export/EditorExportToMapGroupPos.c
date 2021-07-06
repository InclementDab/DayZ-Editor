class EditorExportToMapGroupPos: EditorExportCommandBase
{
	override typename GetFileType() 
	{
		return EditorMapGroupPosFile;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_EXPORT_POS";
	}
}