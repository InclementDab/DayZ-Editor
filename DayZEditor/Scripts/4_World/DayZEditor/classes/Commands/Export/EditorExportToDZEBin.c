class EditorExportToDZEBin: EditorExportCommandBase
{
	override string GetName()
	{
		return "Export to DZE Bin (*.dzebin)";
	}
	
	override typename GetFileType() 
	{
		return EditorDZEBinFile;
	}
}