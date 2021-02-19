class EditorExportToCOM: EditorExportCommandBase
{
	override typename GetFileType() 
	{
		return EditorCOMFile;
	}
	
	override string GetName() 
	{
		return "Export to COM File";
	}
}