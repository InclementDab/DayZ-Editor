
class EditorCameraMapMarker: EditorWidgetEventHandler
{
	protected EditorCamera m_ActiveCamera = null;	
	
	private ref MapWidget m_MapWidget;
	
	override void Update(float timeslice)
	{
		if (m_Root == null) return;
		if (m_ActiveCamera == null) return;

		vector pos = m_MapWidget.MapToScreen(m_ActiveCamera.GetPosition());
		
		m_Root.SetPos(pos[0], pos[1]);
		vector ypr = m_ActiveCamera.GetYawPitchRoll();
		
		m_Root.SetRotation(0, 0, ypr[0] - 90);
	}
	
	void SetCamera(EditorCamera camera, MapWidget map_widget, ScriptInvoker update_invoker)
	{
		Print("EditorCameraMapMarker::SetCamera");
		m_ActiveCamera = camera;
		m_MapWidget = map_widget;
		update_invoker.Insert(Update);
	}
}