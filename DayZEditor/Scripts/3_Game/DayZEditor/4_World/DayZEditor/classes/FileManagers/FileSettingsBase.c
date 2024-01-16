class FileSettingsBase
{
	protected typename m_FileType;
	void SetFileType(typename type)
	{
		m_FileType = type;
	}
	
	string GetSettingsName();
	
	void GetFileSettings(out array<ref ScriptView> settings);
}