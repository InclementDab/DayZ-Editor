class EditorHud: ScriptViewTemplate<EditorHudController>
{
	const float DEFAULT_BAR_WIDTH_PX = 340.0;

	const float BAR_WIDTH_MINIMUM_PX = 100.0;
	const float BAR_WIDTH_MAXIMUM_PX = 900.0;

	protected bool m_IsBoxSelectActive;
	
	// Layout Elements
	Widget NotificationFrame;
	Widget MapContainer;
	Widget LoggerFrame;

	Widget LeftbarWrapper, RightbarWrapper;
	Widget LeftbarDrag, RightbarDrag;
	Widget LeftbarDrag0, RightbarDrag0;
	
	Widget RightbarCollapsePanel, LeftbarCollapsePanel;

	protected Widget m_DragWidget;
	
	CanvasWidget EditorCanvas;
	
	ref EditorCameraMapMarker CameraMapMarker;
	
	// todo protect this and move all Map logic in here?
	MapWidget EditorMapWidget;
	
	EditBoxWidget LeftbarSearchBar;
	
	void EditorHud()
	{	
		EditorLog.Trace("EditorHud");
		EditorMapWidget.Show(false);
		
		ShowScreenLogs(GetEditor().Settings.ShowScreenLogs);
	}
	
	void ~EditorHud()
	{
		delete CameraMapMarker;
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
				
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);

		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);

		Input input = GetGame().GetInput();
		UAInputAPI input_api = GetUApi();
		
		Widget widget_under_cursor = GetWidgetUnderCursor();

		if (input.LocalPress("EditorToggleUI")) {
			if (GetEditor().IsInventoryEditorActive()) {
				GetEditor().GetInventoryEditorHud().GetLayoutRoot().Show(!GetEditor().GetInventoryEditorHud().GetLayoutRoot().IsVisible());
				return;
			}
			
			Show(!IsVisible());
		}

		float wr_s_w, wr_s_h, wr_col_s_w, wr_col_s_h;
		switch (widget_under_cursor) {
			case LeftbarDrag: {
				if (input_api.GetInputByID(UAFire).LocalPress()) {
					m_DragWidget = LeftbarWrapper;
				}
				
				if (input_api.GetInputByID(UAFire).LocalDoubleClick()) {
					LeftbarWrapper.GetSize(wr_s_w, wr_s_h);
					LeftbarWrapper.SetSize(DEFAULT_BAR_WIDTH_PX, wr_s_h);
				}
				
				break;
			}

			case RightbarDrag: {
				if (input_api.GetInputByID(UAFire).LocalPress()) {
					m_DragWidget = RightbarWrapper;
				}

				if (input_api.GetInputByID(UAFire).LocalDoubleClick()) {
					LeftbarWrapper.GetSize(wr_s_w, wr_s_h);
					RightbarWrapper.SetSize(DEFAULT_BAR_WIDTH_PX, wr_s_h);
				}

				break;
			}
		}
		
		if (input_api.GetInputByID(UAFire).LocalRelease()) {
			m_DragWidget = null;
		}

		if (m_DragWidget) {
			switch (m_DragWidget) {
				case LeftbarWrapper: {
					LeftbarDrag0.SetColor(LinearColor.SLATE_BLUE);
					LeftbarCollapsePanel.GetScreenSize(wr_col_s_w, wr_col_s_h);
					LeftbarWrapper.GetScreenSize(wr_s_w, wr_s_h);
					LeftbarWrapper.SetScreenSize(Math.Clamp(mouse_x + wr_col_s_w, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX), wr_s_h);
					break;
				}

				case RightbarWrapper: {
					RightbarDrag0.SetColor(LinearColor.SLATE_BLUE);
					RightbarCollapsePanel.GetScreenSize(wr_col_s_w, wr_col_s_h);
					RightbarWrapper.GetScreenSize(wr_s_w, wr_s_h);
					RightbarWrapper.SetScreenSize(Math.Clamp(screen_x - mouse_x - wr_col_s_w, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX), wr_s_h);
					break;
				}
			}			
									
		} else {
			if (widget_under_cursor == LeftbarDrag || widget_under_cursor == RightbarDrag) {
				widget_under_cursor.GetChildren().SetColor(LinearColor.LIGHT_GRAY);
			} else {
				LeftbarDrag0.SetColor(LinearColor.Create(40, 40, 40));
				RightbarDrag0.SetColor(LinearColor.Create(40, 40, 40));
			}
		}
	}

	override void Show(bool show) 
	{
		super.Show(show);
		
		if (CurrentDialog) {
			CurrentDialog.GetLayoutRoot().Show(show);
		}
	}
		
	void ToggleCursor() 
	{		
		ShowCursor(!GetGame().GetUIManager().IsCursorVisible());
	}
	
	void ShowCursor(bool state) 
	{
		GetGame().GetUIManager().ShowCursor(state);
		
		if (!state) {
			delete CurrentTooltip;
			delete CurrentMenu;
			SetFocus(null);
		}
	}
		
	void ShowScreenLogs(bool state)
	{
		LoggerFrame.Show(state);
	}
		
	void CreateNotification(string text, int color = -4301218, float duration = 4)
	{
		EditorLog.Trace("EditorHud::CreateNotification");
		
		EditorNotification notification = new EditorNotification(text, color);
		
		notification.Play(duration);
	}
	
	bool IsMapVisible()
	{
		return EditorMapWidget.IsVisible();
	}
	
	bool IsSelectionBoxActive()
	{
		return m_IsBoxSelectActive;
	}
	
	void DelayedDragBoxCheck()
	{
		if (!IsVisible() || !GetGame().GetInput().HasGameFocus()) { 
			return;
		}

		int x, y;
		x += 6;
		GetMousePos(x, y);
		thread _DelayedDragBoxCheck(x, y);		
	}
	
	void ScrollToListItem(EditorListItem list_item)
	{
		
		//VScrollToWidget(list_item.GetLayoutRoot());
	}
	
	private void _DelayedDragBoxCheck(int start_x, int start_y)
	{
		int drag_box_color = GetEditor().Settings.SelectionColor;
		
		int a, r, g, b;
		InverseARGB(drag_box_color, a, r, g, b);
		int drag_box_color_fill = ARGB(50, r, g, b);			
		
		int current_x, current_y;
		while ((GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) && GetGame().GetInput().HasGameFocus()) {			
			m_IsBoxSelectActive = true;
			GetMousePos(current_x, current_y);
			// @Sumrak :ANGERY:
			current_x += 6;
			
			EditorCanvas.Clear();
			g_Editor.ClearSelection();
			
			// Draw Drag Box
			if (Math.AbsInt(start_x - current_x) > DRAG_BOX_THRESHOLD || Math.AbsInt(start_y - current_y) > DRAG_BOX_THRESHOLD) {
				EditorCanvas.DrawLine(start_x, start_y, current_x, start_y, DRAG_BOX_THICKNESS, drag_box_color);
				EditorCanvas.DrawLine(start_x, start_y, start_x, current_y, DRAG_BOX_THICKNESS, drag_box_color);
				EditorCanvas.DrawLine(start_x, current_y, current_x, current_y, DRAG_BOX_THICKNESS, drag_box_color);
				EditorCanvas.DrawLine(current_x, start_y, current_x, current_y, DRAG_BOX_THICKNESS, drag_box_color);
				
				// Handles the fill operation
				int x_avg = (start_x + current_x) / 2;
				EditorCanvas.DrawLine(x_avg, start_y, x_avg, current_y, current_x - start_x, drag_box_color_fill); 
				
				EditorObjectMap placed_objects = g_Editor.GetPlacedObjects();
				foreach (EditorObject editor_object: placed_objects) {					
					float marker_x, marker_y;
					EditorObjectMarker object_marker = editor_object.GetMarker();
					if (object_marker) {
						object_marker.GetPos(marker_x, marker_y);
						
						//i think only checking if within cone of box select not distance
						if ((marker_x < Math.Max(start_x, current_x) && marker_x > Math.Min(start_x, current_x)) && (marker_y < Math.Max(start_y, current_y) && marker_y > Math.Min(start_y, current_y))) {
							//check if within markerviewdistance to allow selection.
							if (vector.Distance(editor_object.GetPosition(), g_Editor.GetCamera().GetPosition()) <= g_Editor.Settings.MarkerViewDistance) {
								g_Editor.SelectObject(editor_object);
							}
						}
					}
				}		
			}
			
			Sleep(10);
		}
		
		m_IsBoxSelectActive = false;
		EditorCanvas.Clear();
	}

	void ShowRuleOfThirds(bool state)
	{
		if (!state) {
			EditorCanvas.Clear();
			return;
		}
		
		int x, y;

		GetScreenSize(x, y);				
		EditorCanvas.DrawLine(x / 3, 0, x / 3, y, 1, COLOR_BLACK);
		EditorCanvas.DrawLine((x / 3) * 2, 0, (x / 3) * 2, y, 1, COLOR_BLACK);
		
		EditorCanvas.DrawLine(0, y / 3, x, y / 3, 1, COLOR_BLACK);
		EditorCanvas.DrawLine(0, (y / 3) * 2, x, (y / 3) * 2, 1, COLOR_BLACK);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/layouts/hud/EditorHud.layout";
	}
	
	// Modal Menu Control
	static ref EditorMenu CurrentMenu;
	
	// ToolTip Control
	protected ref ScriptView CurrentTooltip;
	void SetCurrentTooltip(ScriptView current_tooltip) 
	{
		delete CurrentTooltip;
		
		// Dont create a tooltip if conditions are met
		if (IsSelectionBoxActive()) {
			return;
		}
		
		CurrentTooltip = current_tooltip;
	}
		
	// Dialog Control
	static ref DialogBase CurrentDialog;
	
	static bool IsDialogCommand(Widget w) 
	{
		return (CurrentDialog && CurrentDialog.GetLayoutRoot() && CurrentDialog.GetLayoutRoot().FindAnyWidget(w.GetName()));
	}
	
	protected ref map<typename, vector> m_LastDialogPosition = new map<typename, vector>();
	
	void RegisterLastDialogPosition(ScriptView dialog)
	{
		float x, y;
		dialog.GetLayoutRoot().GetPos(x, y);
		m_LastDialogPosition[dialog.Type()] = Vector(x, y, 0);
	}
	
	vector GetLastDialogPosition(ScriptView dialog)
	{
		return m_LastDialogPosition[dialog.Type()];
	}
}
