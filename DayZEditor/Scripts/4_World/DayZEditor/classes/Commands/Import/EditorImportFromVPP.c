class EditorImportFromVPP: EditorImportCommandBase
{
	override string GetName() 
	{
		return "Import from .VPP";
	}
	
	override typename GetFileType() 
	{
		return EditorVPPFile;
	}
}