class ExportSettings: FileSettingsBase
{
	bool Binarized;
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
		}
	}
	
	void PropertyChanged(string property_name)
	{
		Print(property_name);
	}
}