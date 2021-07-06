class EditorImportFromInit: EditorImportCommandBase
{
	override string GetName() 
	{
		return "Import from init.c";
	}
	
	override typename GetFileType() 
	{
		return EditorInitFile;
	}
}