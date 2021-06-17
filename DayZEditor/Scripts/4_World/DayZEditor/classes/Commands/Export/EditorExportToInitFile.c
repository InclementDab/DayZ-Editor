class EditorExportToInitFile: EditorExportCommandBase
{	
	override typename GetFileType() 
	{
		return EditorInitFile;
	}
	
	override string GetName() 
	{
		return "Export to init.c";
	}
}