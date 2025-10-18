class EditorFileType: Managed
{
	EditorSaveData Import(string file, ImportSettings settings) { return null; }
	void Export(EditorSaveData data, string file, ExportSettings settings, eDialogExtraSetting dialog_setting) {}
	
	Managed Import(string file) { return null; }
	void Export(Managed data, string file);
	
	eDialogExtraSetting GetExportSettings()
	{
		return eDialogExtraSetting.EXPORT_SELECTED_ONLY;
	}
	
	string GetExtension()
	{
		return ".txt";
	}
	
	void GetValidExtensions(notnull inout array<ref Param2<string, string>> valid_extensions)
	{
		//valid_extensions.Insert(new Param2<string, string>("Any", "*.*"));
	}
	
	bool CanDoDeletion()
	{
		return false;
	}
}