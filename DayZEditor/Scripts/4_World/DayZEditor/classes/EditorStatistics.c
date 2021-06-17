class EditorStatistics
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
	
	int PlayTime;
	int PlacedObjects;
	int RemovedObjects;
	
	void Load()
	{
		PlayTime = EditorSettings.GetProfileInt("EditorPlayTime", PlayTime);
		PlacedObjects = EditorSettings.GetProfileInt("EditorPlacedObjects", PlacedObjects);
		RemovedObjects = EditorSettings.GetProfileInt("EditorRemovedObjects", RemovedObjects);
	}
	
	void Save()
	{
		EditorSettings.SetProfileInt("EditorPlayTime", PlayTime);
		EditorSettings.SetProfileInt("EditorPlacedObjects", PlacedObjects);
		EditorSettings.SetProfileInt("EditorRemovedObjects", RemovedObjects);
	}
}