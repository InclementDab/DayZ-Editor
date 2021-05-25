class EditorCameraTrackManager
{
	ref ScriptInvoker OnTrackStart = new ScriptInvoker;
	ref ScriptInvoker OnTrackStop = new ScriptInvoker;
	
	ref array<ref EditorCameraTrackListItem> CameraTracks = {};
	ref array<EditorCameraTrackListItem> SelectedCameraTracks = {};
	
	protected bool m_CameraTrackRunning;
	protected vector m_CameraTrackStartPosition;
	protected vector m_CameraTrackStartOrientation;
	
	void InsertCameraTrack(EditorCamera camera, float time, string name)
	{
		EditorLog.Trace("EditorCameraTrackManager::InsertCameraTrack");
		InsertCameraTrack(new EditorCameraTrackListItem(camera.GetPosition(), camera.GetOrientation(), time, name));
	}
	
	void InsertCameraTrack(EditorCameraTrackListItem camera_track_item)
	{
		EditorLog.Trace("EditorCameraTrackManager::InsertCameraTrack");
		CameraTracks.Insert(camera_track_item);
		GetEditor().GetEditorHud().GetTemplateController().CameraTrackData.Insert(camera_track_item);
		
		EditorAction action = new EditorAction("DeleteCameraTrack", "CreateCameraTrack");
		action.InsertUndoParameter(camera_track_item.GetSerializedData());
		action.InsertRedoParameter(camera_track_item.GetSerializedData());
		
		GetEditor().InsertAction(action);
	}
		
	void DeleteCameraTrack(EditorCameraTrackListItem camera_track_item)
	{
		EditorLog.Trace("EditorCameraTrackManager::RemoveCameraTrack");
		
		EditorAction action = new EditorAction("CreateCameraTrack", "DeleteCameraTrack");
		action.InsertUndoParameter(camera_track_item.GetSerializedData());
		action.InsertRedoParameter(camera_track_item.GetSerializedData());
		
		CameraTracks.RemoveItem(camera_track_item);
		GetEditor().GetEditorHud().GetTemplateController().CameraTrackData.Remove(GetEditor().GetEditorHud().GetTemplateController().CameraTrackData.Find(camera_track_item));
		
		GetEditor().InsertAction(action);
	}
	
	void DeleteCameraTracks(array<EditorCameraTrackListItem> camera_track_item)
	{
		EditorLog.Trace("EditorCameraTrackManager::RemoveCameraTrack");
		
	}
	
	void SelectCameraTrack(EditorCameraTrackListItem camera_track_item)
	{
		EditorLog.Trace("EditorCameraTrackManager::SelectedCameraTrack");
		SelectedCameraTracks.Insert(camera_track_item);
		camera_track_item.OnSelected();
	}
	
	void DeselectCameraTrack(EditorCameraTrackListItem camera_track_item)
	{
		EditorLog.Trace("EditorCameraTrackManager::InsertCameraTrack");
		camera_track_item.OnDeselected();
		SelectedCameraTracks.RemoveOrdered(SelectedCameraTracks.Find(camera_track_item));
	}
	
	void ClearSelection()
	{
		EditorLog.Trace("EditorCameraTrackManager::ClearSelection");
		foreach (EditorCameraTrackListItem list_item: SelectedCameraTracks) {
			DeselectCameraTrack(list_item);
		}
	}
	
	array<ref EditorCameraTrackListItem> GetCameraTracks()
	{
		return CameraTracks;
	}
	
	array<EditorCameraTrackListItem> GetSelectedTracks()
	{
		array<EditorCameraTrackListItem> result = {};
		foreach (EditorCameraTrackListItem selected: SelectedCameraTracks) {
			result.Insert(selected);
		}
		
		return result;
	}
	
	int CameraTrackCount()
	{
		return CameraTracks.Count();
	}
	
	void Start()
	{
		m_CameraTrackRunning = true;
		m_CameraTrackStartPosition = GetEditor().GetCamera().GetPosition();
		m_CameraTrackStartOrientation = GetEditor().GetCamera().GetOrientation();
			
		thread _RunCameraTrack();
	}
	
	private void _RunCameraTrack()
	{
		m_CameraTrackRunning = true;
		EditorCamera camera = GetEditor().GetCamera();
		for (int i = 0; i < CameraTracks.Count(); i++) {
			if (!m_CameraTrackRunning) return; // cancel
			EditorCameraTrackListItemController start_ctrl = CameraTracks[i].GetData();
			if (!CameraTracks[i + 1]) {
				continue;
			}
			
			EditorCameraTrackListItemController end_ctrl = CameraTracks[i + 1].GetData();			
			
			camera.SetPosition(start_ctrl.GetPosition());
			camera.SetOrientation(start_ctrl.GetOrientation());
			
			int td = 0;
			while (td <= start_ctrl.Time * 1000) {
				if (!m_CameraTrackRunning) return; // cancel
				float time_value = 1 / (start_ctrl.Time * 1000) * td;
				//vector center_point = AverageVectors(ctrl.GetPosition(), next_ctrl.GetPosition()) + vector.Up * ((CameraTrackSmoothing / 100) * vector.Distance(ctrl.GetPosition(), next_ctrl.GetPosition()));
				//point = EditorMath.CalculateQuadraticBezierPoint(time_value, start_ctrl.GetPosition(), center_ctrl.GetPosition(), next_ctrl.GetPosition());
				
				vector position = EditorMath.LerpVector(start_ctrl.GetPosition(), end_ctrl.GetPosition(), time_value);
				camera.SetPosition(position);
				
				vector orientation = EditorMath.SmoothLerpVector(start_ctrl.GetOrientation(), end_ctrl.GetOrientation(), time_value);
				camera.SetOrientation(orientation);

				td += 10;
				Sleep(10);
			}
		}
		
		Stop();
	}
	
	void Stop()
	{
		m_CameraTrackRunning = false;
		GetEditor().GetCamera().SetPosition(m_CameraTrackStartPosition);
		GetEditor().GetCamera().SetPosition(m_CameraTrackStartOrientation);

	}
	
	bool IsRunning()
	{
		return m_CameraTrackRunning;
	}
	
	// ??
	void Pause()
	{
		
	}
}