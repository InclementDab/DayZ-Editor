class FileSettingsBase: Managed
{
	protected ref EditorFileType m_FileType;
	void SetFileType(typename type)
	{
		if (!type.IsInherited(EditorFileType)) {
			Error("Invalid type specified");
			return;
		}

		m_FileType = EditorFileType.Cast(type.Spawn());
	}
	
	EditorFileType GetFileType()
	{
		return m_FileType;
	}
	
	string GetSettingsName()
	{
		return string.Empty;
	}
	
	void GetFileSettings(out array<ref ScriptView> settings)
	{
	}
}