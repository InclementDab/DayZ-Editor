class ExportSettings: FileSettingsBase
{
	bool Binarized = true;
	bool ExportEntireMap;
	ExportHeightType HeightType;
	bool ExportSelectedOnly;
	vector ExportOffset;
	string ExportSetName;
	
	override void GetFileSettings(out array<ref ScriptView> settings)
	{
		switch (m_FileType) {
			case EditorDZEFile: {
				settings.Insert(new CheckBoxPrefab("Binarized", this, "Binarized"));
				break;
			}
			
			case EditorMapGroupPosFile: {
				settings.Insert(new CheckBoxPrefab("Export Entire Map", this, "ExportEntireMap"));
				break;
			}
		}
		
		settings.Insert(new CheckBoxPrefab("Export Selected Only", this, "ExportSelectedOnly"));
	}
	
	override string GetSettingsName()
	{
		return "Export Settings";
	}
}