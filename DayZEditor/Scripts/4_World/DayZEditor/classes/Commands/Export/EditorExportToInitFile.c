class EditorExportToInitFile: EditorExportCommandBase
{	
	override typename GetFileType() 
	{
		return EditorInitFile;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_EXPORT_INIT_C";
	}
}