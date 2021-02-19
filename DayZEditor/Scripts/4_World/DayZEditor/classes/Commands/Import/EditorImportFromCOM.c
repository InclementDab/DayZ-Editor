class EditorImportFromCOM: EditorImportCommandBase
{
	override string GetName() 
	{
		return "Import from COM";
	}
	
	override typename GetFileType() 
	{
		return EditorCOMFile;
	}
}