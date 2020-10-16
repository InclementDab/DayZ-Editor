class EditorHud: ScriptViewTemplate<EditorHudController>
{
	// Layout Elements
	protected Widget NotificationFrame;
	protected Widget MapContainer;
	
	protected CanvasWidget EditorCanvas;
	
	// todo protect this and move all Map logic in here?
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
	
	void DelayedDragBoxCheck()
	{
		if (!IsVisible()) return;
		
		int x, y;
		x += 6;
		GetMousePos(x, y);
		thread _DelayedDragBoxCheck(x, y);
	}
	
	private void _DelayedDragBoxCheck(int start_x, int start_y)
	{
		int current_x, current_y;
		while (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) {
			GetMousePos(current_x, current_y);
			// @Sumrak :ANGERY:
			current_x += 6;
			
			EditorCanvas.Clear();
			
			// Draw Drag Box
			if (Math.AbsInt(start_x - current_x) > DRAG_BOX_THRESHOLD || Math.AbsInt(start_y - current_y) > DRAG_BOX_THRESHOLD) {
				EditorCanvas.DrawLine(start_x, start_y, current_x, start_y, DRAG_BOX_THICKNESS, DRAG_BOX_COLOR);
				EditorCanvas.DrawLine(start_x, start_y, start_x, current_y, DRAG_BOX_THICKNESS, DRAG_BOX_COLOR);
				EditorCanvas.DrawLine(start_x, current_y, current_x, current_y, DRAG_BOX_THICKNESS, DRAG_BOX_COLOR);
				EditorCanvas.DrawLine(current_x, start_y, current_x, current_y, DRAG_BOX_THICKNESS, DRAG_BOX_COLOR);
							
				EditorObjectMap placed_objects = g_Editor.GetPlacedObjects();
				foreach (EditorObject editor_object: placed_objects) {
					
					if (editor_object.IsSelected()) continue;
					
					float marker_x, marker_y;
					EditorObjectMarker object_marker = editor_object.GetMarker();
					if (object_marker) {
						object_marker.GetPos(marker_x, marker_y);
						
						if ((marker_x < Math.Max(start_x, current_x) && marker_x > Math.Min(start_x, current_x)) && (marker_y < Math.Max(start_y, current_y) && marker_y > Math.Min(start_y, current_y))) {		
							g_Editor.SelectObject(editor_object);
						}
					}
				}		
			}
			
			Sleep(10);
		}
		
		EditorCanvas.Clear();
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/hud/EditorHud.layout";
	}
}
