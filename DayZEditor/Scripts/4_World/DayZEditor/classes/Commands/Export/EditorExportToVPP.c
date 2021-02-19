class EditorExportToVPP: EditorExportCommandBase
{
	override typename GetFileType() 
	{
		return EditorVPPFile;
	}
	
	override string GetName() 
	{
		return "Export to VPP";
	}
}