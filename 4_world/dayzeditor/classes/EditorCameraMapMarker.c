class EditorCameraMapMarker: ScriptedWidgetEventHandler
{
	protected ref Widget m_Root;
	protected ImageWidget m_EditorMapMarkerImage;
	
	protected EditorCamera m_ActiveCamera;
	
	void ~EditorCameraMapMarker()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		delete m_Root; delete m_EditorMapMarkerImage;
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		Print("EditorCameraMapMarker::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		
		m_EditorMapMarkerImage = ImageWidget.Cast(m_Root.FindAnyWidget("EditorMapMarkerImage"));	
	}
	
	void Update()
	{
		
		MapWidget map_widget = MapWidget.Cast(m_Root.GetParent());
		vector pos = map_widget.MapToScreen(m_ActiveCamera.GetPosition());
		
		m_Root.SetPos(pos[0], pos[1]);
		vector ypr = m_ActiveCamera.GetYawPitchRoll();
		
		m_Root.SetRotation(0, 0, ypr[0] - 90);
	}
	
	void SetCamera(EditorCamera camera)
	{
		m_ActiveCamera = camera;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
}