class EditorHud: ScriptViewTemplate<EditorHudController>
{	
	// Layout Elements
	protected Widget NotificationFrame;
	protected Widget MapContainer;
	
	MapWidget EditorMapWidget;

	void EditorHud()
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
			if (placed_objects) {
				foreach (EditorObject editor_object: placed_objects) {
					editor_object.Show(show);
				}
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
		
		EditorNotification notification = new EditorNotification(text, color);
		notification.SetParent(NotificationFrame);
		notification.Play(duration);
	}
	
	bool IsMapVisible()
	{
		return EditorMapWidget.IsVisible();
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/hud/EditorHud.layout";
	}
}
