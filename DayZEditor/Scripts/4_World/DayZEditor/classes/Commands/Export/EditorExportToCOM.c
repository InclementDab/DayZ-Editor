class EditorExportToCOM: EditorExportCommandBase
{
	override typename GetFileType() 
	{
		return EditorCOMFile;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_EXPORT_COM";
	}
}