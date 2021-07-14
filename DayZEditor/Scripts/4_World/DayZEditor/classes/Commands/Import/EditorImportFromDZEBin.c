class EditorImportFromDZEBin: EditorImportCommandBase
{
	override string GetName()
	{
		return "Import from DZE Bin (*.dzebin)";
	}
	
	override typename GetFileType() 
	{
		return EditorDZEBinFile;
	}
}