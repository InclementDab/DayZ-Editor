class EditorExportToVPP: EditorExportCommandBase
{
	override typename GetFileType() 
	{
		return EditorVPPFile;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_EXPORT_VPP";
	}
}