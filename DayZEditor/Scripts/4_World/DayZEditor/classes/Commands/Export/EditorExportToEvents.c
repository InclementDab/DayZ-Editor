class EditorExportToEvents: EditorExportCommandBase
{
	override typename GetFileType() 
	{
		return EditorEventsFile;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_EXPORT_EVENTS";
	}
}