class EditorFileType
{
	EditorSaveData Import(string file, ImportSettings settings);
	void Export(EditorSaveData data, string file, ExportSettings settings);
	
	string GetExtension();
	
	bool CanDoDeletion()
	{
		return false;
	}
}