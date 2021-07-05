class EditorStatistics: EditorProfileSettings
{
	protected static ref EditorStatistics m_Instance;
	
	static EditorStatistics GetInstance()
	{
		if (!m_Instance) {
			m_Instance = new EditorStatistics();
			m_Instance.Load();
		}
		
		return m_Instance;
	}
	
	int EditorPlayTime;
	int EditorPlacedObjects;
	int EditorRemovedObjects;
}