class EditorCameraTrack: EditorObject
{
	protected ref EditorCameraTrackListItem m_CameraListItem;

	void ~EditorCameraTrack()
	{
		delete m_CameraListItem;
	}
	
	void SetListIndex(int index)
	{
		if (m_CameraListItem) {
			int old_index = GetEditor().GetEditorHud().GetTemplateController().CameraTrackData.Find(m_CameraListItem);
			GetEditor().GetEditorHud().GetTemplateController().CameraTrackData.Remove(old_index);
			//GetEditor().GetEditorHud().GetTemplateController().CameraTrackData.InsertAt(m_CameraListItem, index);
		}
	}

	override void EnableListItem(bool enable) 
	{		
		delete m_CameraListItem;
		
		if (!enable) {
			return;
		}
		
		m_CameraListItem = new EditorCameraTrackListItem(this);
		GetEditor().GetEditorHud().GetTemplateController().CameraTrackData.Insert(m_CameraListItem);
	}
}


/*

class EditorCameraTrackManagerModule: JMModuleBase
{
	ref ScriptInvoker OnTrackStart;
	ref ScriptInvoker OnTrackStop;
	
	
	protected bool m_CameraTrackRunning;
	protected vector m_CameraTrackStartPosition;
	protected vector m_CameraTrackStartOrientation;
	
	override void Init()
	{
		OnTrackStart = new ScriptInvoker();
		OnTrackStop = new ScriptInvoker();
		
		CameraTracks = {};
		SelectedCameraTracks = {};
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
				
				vector position = Math.LerpVector(start_ctrl.GetPosition(), end_ctrl.GetPosition(), time_value);
				camera.SetPosition(position);
				
				vector orientation = Math.SmoothLerpVector(start_ctrl.GetOrientation(), end_ctrl.GetOrientation(), time_value);
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
		//GetEditor().GetCamera().SetPosition(m_CameraTrackStartPosition);
		//GetEditor().GetCamera().SetPosition(m_CameraTrackStartOrientation);
	}
	
	bool IsRunning()
	{
		return m_CameraTrackRunning;
	}
	
	// ??
	void Pause()
	{
		
	}
	
	override bool IsClient() 
	{	
		return true;
	}
	
	override bool IsServer() 
	{
		return false;
	}
}*/