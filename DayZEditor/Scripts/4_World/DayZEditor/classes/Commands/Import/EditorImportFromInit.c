class EditorImportFromInit: EditorImportCommandBase
{
	override string GetName() 
	{
		return "Import from Init.c";
	}
	
	override typename GetFileType() 
	{
		return EditorInitFile;
	}
}