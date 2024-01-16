class EditorImportFromVPP: EditorImportCommandBase
{
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_IMPORT_VPP (*.vpp)";
	}
	
	override typename GetFileType() 
	{
		return EditorVPPFile;
	}
}