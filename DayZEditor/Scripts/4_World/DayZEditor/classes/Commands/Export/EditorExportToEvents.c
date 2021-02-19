class EditorExportToEvents: EditorExportCommandBase
{
	override typename GetFileType() 
	{
		return EditorEventsFile;
	}
	
	override string GetName() 
	{
		return "Export to Events File";
	}
}