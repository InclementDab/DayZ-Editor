class EditorCameraMapMarker: EditorWidgetEventHandler
{
	protected ImageWidget m_EditorMapMarkerImage;
	protected EditorCamera m_ActiveCamera = null;
	
	
	override void Update(float timeslice)
	{
		if (m_Root == null) return;
		if (m_ActiveCamera == null) return;
		MapWidget m_MapWidget = GetEditor().GetUIManager().GetEditorUI().GetMapWidget();
		vector pos = m_MapWidget.MapToScreen(m_ActiveCamera.GetPosition());
		
		m_Root.SetPos(pos[0], pos[1]);
		vector ypr = m_ActiveCamera.GetYawPitchRoll();
		
		m_Root.SetRotation(0, 0, ypr[0] - 90);
	}
	
	void SetCamera(EditorCamera camera)
	{
		m_ActiveCamera = camera;
		GetEditor().GetUIManager().GetUpdateInvoker().Insert(Update);
	}
}