

class EditorHud: ScriptView
{	
	// Layout Elements
	protected Widget NotificationFrame;
	protected Widget MapContainer;
	
	MapWidget EditorMapWidget;
	
	
	void EditorHud(Widget parent = null)
	{	
		EditorLog.Trace("EditorHud");
		EditorMapWidget.Show(false);
	}

	
	void Show(bool show) 
	{
		EditorLog.Trace("EditorHud::Show");
		m_LayoutRoot.Show(show);
		
		if (GetEditor()) {
			EditorObjectSet placed_objects = GetEditor().GetPlacedObjects();
			foreach (EditorObject editor_object: placed_objects) {
				editor_object.Show(show);
			}
		}
	}
	
	bool IsVisible() {
		return m_LayoutRoot.IsVisible();
	}
	
	void ToggleCursor() {
		ShowCursor(!GetGame().GetUIManager().IsCursorVisible());
	}
	
	void ShowCursor(bool state) {
		GetGame().GetUIManager().ShowCursor(state);
	}
		
	void CreateNotification(string text, int color = -4301218, float duration = 4)
	{
		EditorLog.Trace("EditorHud::CreateNotification");
		
		EditorNotification notification = new EditorNotification(NotificationFrame, text, color);
		notification.Play(duration);
	}
	
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/EditorHud.layout";
	}
	
	override typename GetControllerType() {
		return EditorHudController;
	}
	
	EditorHudController GetEditorHudController() { 
		return EditorHudController.Cast(GetController());
	}
}