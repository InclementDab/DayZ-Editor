class EditorExportToMapGroupPos: EditorExportCommandBase
{
	override typename GetFileType() 
	{
		return EditorMapGroupPosFile;
	}
	
	override string GetName() 
	{
		return "Export to MapGroupPos";
	}
}