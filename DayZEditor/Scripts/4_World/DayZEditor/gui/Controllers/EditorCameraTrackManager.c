class EditorCameraTrackManager
{
	ref ScriptInvoker OnTrackStart = new ScriptInvoker;
	ref ScriptInvoker OnTrackStop = new ScriptInvoker;
	
	// Binding
	float CameraSmoothing = 50.0;
	ref ObservableCollection<ref EditorCameraTrackListItem> CameraTrackData = new ObservableCollection<ref EditorCameraTrackListItem>(this);
	
	void InsertCameraTrack(EditorCamera camera)
	{
		
	}
	
	void InsertCameraTrack(EditorCameraTrackListItem camera_track_item)
	{
		CameraTrackData.Insert(camera_track_item);
	}
	
	void RemoveCameraTrack()
	{
		
	}
	
	void RemoveCameraTrack(EditorCameraTrackListItem camera_track_item)
	{
		CameraTrackData.Remove(CameraTrackData.Find(camera_track_item));
	}
	
	void SelectCameraTrack()
	{
		
	}
	
	void DeselectCameraTrack()
	{
		
	}
	
	void ClearSelection()
	{
		
	}
	
	int CameraTrackCount()
	{
		return CameraTrackData.Count();
	}
	
	void Start()
	{
		m_CameraTrackStartPosition = GetEditor().GetCamera().GetPosition();
		m_CameraTrackStartOrientation = GetEditor().GetCamera().GetOrientation();
			
		thread _RunCameraTrack();
	}
	
	void Stop()
	{
		GetEditor().GetCamera().SetPosition(m_CameraTrackStartPosition);
		GetEditor().GetCamera().SetPosition(m_CameraTrackStartOrientation);
		

	}
	
	// ??
	void Pause()
	{
		
	}
}