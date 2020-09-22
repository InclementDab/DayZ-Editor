


class EditorHud: MVCLayout
{
	
	protected Widget NotificationFrame;
	
	Widget MapContainer;
	MapWidget Map;
			
	void EditorHud(Widget parent = null)
	{	
		EditorLog.Trace("EditorHud");
	}
	
	void Update(float timeslice);
	
	void Show(bool show) 
	{
		EditorLog.Trace("EditorHud::Show");
		m_LayoutRoot.Show(show);
		SetCursor(show);
		
		EditorObjectSet placed_objects = GetEditor().GetPlacedObjects();
		foreach (EditorObject editor_object: placed_objects) {
			editor_object.Show(show);
		}
	}
	
	bool IsVisible() {
		return m_LayoutRoot.IsVisible();
	}
	
	void SetCursor(bool state) {
		GetGame().GetUIManager().ShowCursor(state);
	}
	
	void ToggleCursor() {
		GetGame().GetUIManager().ShowCursor(!GetGame().GetUIManager().IsCursorVisible());
	}
	
	void ShowCursor() {
		GetGame().GetUIManager().ShowCursor(true);
	}
	
	void HideCursor() {
		GetGame().GetUIManager().ShowCursor(false);
	}
	
	void ShowMap(bool show)	{
		MapContainer.Show(show);
	}
	
	bool IsMapVisible() { 
		return MapContainer.IsVisible(); 
	}
		
	MapWidget GetMap() {
		return GetEditorHudController().Map;
	}
	
	
	void CreateNotification(string text, int color = -4301218, float duration = 4)
	{
		EditorLog.Trace("EditorHud::CreateNotification");
		
		EditorNotification notification = new EditorNotification(NotificationFrame, text, color);
		notification.Play(duration);
	}
	
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/EditorUI.layout";
	}
	
	override typename GetControllerType() {
		return EditorHudController;
	}
	
	EditorHudController GetEditorHudController() { 
		return EditorHudController.Cast(GetController());
	}
}