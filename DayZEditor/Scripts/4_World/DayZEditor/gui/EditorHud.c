enum SelectionMode
{
	BOX,
	ELLIPSE,
	LASSO // ???? are you CRAZY?
};

class EditorHud: ScriptView
{
	const float DEFAULT_BAR_WIDTH_PX = 380.0;

	const float BAR_WIDTH_MINIMUM_PX = 100.0;
	const float BAR_WIDTH_MAXIMUM_PX = 900.0;

	protected bool m_IsBoxSelectActive;
	protected EditorHudController m_TemplateController;
	
	// Layout Elements
	Widget NotificationFrame;
	Widget MapContainer;
	Widget LoggerFrame;

	Widget LeftbarWrapper, RightbarWrapper;
	Widget LeftbarDrag, RightbarDrag;
	Widget LeftbarDrag0, RightbarDrag0;
	
	Widget RightbarCollapsePanel, LeftbarCollapsePanel;

	Widget NotificationPanel;
	TextWidget NotificationText;

	protected Widget m_DragWidget;
	protected int m_DragBoxStartX = -1, m_DragBoxStartY = -1;
	protected float m_DragBoxDelayStart;
	protected SelectionMode m_SelectionMode;
	protected bool m_ObjectSelectToggle;
	
	CanvasWidget EditorCanvas;
	
	ref EditorCameraMapMarker CameraMapMarker;
	
	// todo protect this and move all Map logic in here?
	MapWidget EditorMapWidget;
	
	EditBoxWidget LeftbarSearchBar;

	protected ref array<vector> m_LassoHistory = {};
	ButtonWidget ObjectSelectionButton, BoxSelectionButton, EllipseSelectionButton, LassoSelectionButton;

	void EditorHud(notnull Editor editor)
	{	
		EditorLog.Trace("EditorHud");
		EditorMapWidget.Show(false);
		
		m_TemplateController = EditorHudController.Cast(m_Controller);
						
		ShowScreenLogs(GetEditor().GetSettings().ShowScreenLogs);
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		float s_r_w, s_r_h, s_l_w, s_l_h;
		RightbarWrapper.GetScreenSize(s_r_w, s_r_h);
		RightbarWrapper.SetScreenSize(GetEditor().GetSettings().RightBarPlacement, s_r_h);
		LeftbarWrapper.GetScreenSize(s_l_w, s_l_h);
		LeftbarWrapper.SetScreenSize(GetEditor().GetSettings().LeftBarPlacement, s_l_h);
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
		bool cursor_visible = GetGame().GetUIManager().IsCursorVisible();
		
		ObjectSelectionButton.SetState(m_ObjectSelectToggle);
		BoxSelectionButton.SetState(m_SelectionMode == SelectionMode.BOX);
		EllipseSelectionButton.SetState(m_SelectionMode == SelectionMode.ELLIPSE);
		LassoSelectionButton.SetState(m_SelectionMode == SelectionMode.LASSO);

		if (GetEditor().IsInventoryEditorActive()) {
			Show(false);
			return;
		}
		
		if (input.LocalPress("EditorToggleUI") && (!GetFocus() || !GetFocus().IsInherited(EditBoxWidget))) {		
			Show(!IsVisible());
		}
		
		// Dont want to toggle cursor on map
		if (input.LocalPress("EditorToggleCursor")) {
			if (!EditorMapWidget.IsVisible() && !(EditorHud.CurrentDialog && GetEditor().GetSettings().LockCameraDuringDialogs)) {	
				ToggleCursor();
			}
		}

		if (input.LocalPress("UAFire")) {
			if ((!widget_under_cursor || widget_under_cursor == EditorMapWidget) && GetGame().GetInput().HasGameFocus() && cursor_visible && !GetEditor().IsPlacing() && !GetEditor().IsDragging()) {
				m_DragBoxDelayStart = 0.12;
				GetMousePos(m_DragBoxStartX, m_DragBoxStartY);
				m_LassoHistory.Clear();
			}

			EditorMapWidget.SetFlags(WidgetFlags.IGNOREPOINTER);
		}
		
		if (input.LocalRelease("UAFire")) {
			m_DragBoxDelayStart = 10;
			EditorMapWidget.ClearFlags(WidgetFlags.IGNOREPOINTER);
		}
		
		EditorCanvas.Clear();
		m_DragBoxDelayStart -= dt;
		if (input.LocalValue("UAFire") && m_DragBoxDelayStart < 0 && GetGame().GetInput().HasGameFocus() && cursor_visible && !GetEditor().IsPlacing() && !GetEditor().IsDragging() && !GetEditor().GetBrush() && !m_DragWidget && m_DragBoxStartX != -1 && m_DragBoxStartY != -1) {	
			switch (m_SelectionMode) {
				case SelectionMode.LASSO: {
					vector current = Vector(mouse_x, mouse_y, 0);
					if (m_LassoHistory.Count() > 0) {
						vector last = m_LassoHistory[m_LassoHistory.Count() - 1];
						
						if (vector.Distance(last, current) > 4) {
							m_LassoHistory.Insert(current);
						}				
					} else {
						m_LassoHistory.Insert(current);
					}
					
					for (int j = 0; j < m_LassoHistory.Count() - 1; j++) {
						EditorCanvas.DrawLine(m_LassoHistory[j][0], m_LassoHistory[j][1], m_LassoHistory[j + 1][0], m_LassoHistory[j + 1][1], 4, 0xFF4B77BE);
					}
					
					if (m_LassoHistory.Count() > 2) {
						EditorCanvas.DrawLine(m_LassoHistory[0][0], m_LassoHistory[0][1], m_LassoHistory[m_LassoHistory.Count() - 1][0], m_LassoHistory[m_LassoHistory.Count() - 1][1], 2, 0xFF4B77BE);
					}
					
					foreach (EditorMarker marker0: EditorMarker.s_AllMarkers) {
						if (!marker0 || !marker0.GetLayoutRoot().IsVisible()) {
							continue;
						}

						EditorObjectMarker object_marker0 = EditorObjectMarker.Cast(marker0);
						float x_n0, y_n0;
						marker0.GetLayoutRoot().GetScreenPos(x_n0, y_n0);
						if (IsPointInPolygon(x_n0, y_n0, m_LassoHistory)) {
							if (object_marker0 && !object_marker0.GetEditorObject().IsSelected()) {
								GetEditor().SelectObject(object_marker0.GetEditorObject());
							}
						} else {
							if (object_marker0 && object_marker0.GetEditorObject().IsSelected()) {
								GetEditor().DeselectObject(object_marker0.GetEditorObject());
							}
						}
					}
					
					break;
				}
				
				case SelectionMode.BOX: {
			
					// Rectangle
					int x_avg = (m_DragBoxStartX + mouse_x) / 2;
					int y_avg = (m_DragBoxStartY + mouse_y) / 2;
					EditorCanvas.DrawLine(x_avg, m_DragBoxStartY, x_avg, mouse_y, mouse_x - m_DragBoxStartX, 0x644B77BE);		
					
					foreach (EditorMarker marker: EditorMarker.s_AllMarkers) {
						if (!marker || !marker.GetLayoutRoot().IsVisible()) {
							continue;
						}
						
						EditorObjectMarker object_marker = EditorObjectMarker.Cast(marker);
						
						float x_n, y_n;
						marker.GetLayoutRoot().GetScreenPos(x_n, y_n);
						//if (top_left[0] <= m_screen_x && m_screen_x <= bottom_right[0] && top_left[1] <= m_screen_y && m_screen_y <= bottom_right[1]) {
						if ((x_n < Math.Max(m_DragBoxStartX, mouse_x) && x_n > Math.Min(m_DragBoxStartX, mouse_x)) && (y_n < Math.Max(m_DragBoxStartY, mouse_y) && y_n > Math.Min(m_DragBoxStartY, mouse_y))) {
							if (object_marker && !object_marker.GetEditorObject().IsSelected()) {
								GetEditor().SelectObject(object_marker.GetEditorObject());
							}
						} else {
							if (object_marker && object_marker.GetEditorObject().IsSelected()) {
								GetEditor().DeselectObject(object_marker.GetEditorObject());
							}
						}
					}
					
					break;
				}
				
				case SelectionMode.ELLIPSE: {
					int x1 = Math.Min(mouse_x, m_DragBoxStartX);
					int x2 = Math.Max(mouse_x, m_DragBoxStartX);
					int y1 = Math.Min(mouse_y, m_DragBoxStartY);
					int y2 = Math.Max(mouse_y, m_DragBoxStartY);

					// Calculate dimensions
					float width = x2 - x1;
					float height = y2 - y1;

					// Get center point
					float center_x = (x1 + x2) / 2.0;
					float center_y = (y1 + y2) / 2.0;

					float abs_height = Math.AbsFloat(height);
					float abs_width = Math.AbsFloat(width);
					if (abs_width == 0 || abs_height == 0) {
						break;
					}

					float lineThickness = 0.5;

					// Scan from top to bottom of the ellipse
					for (float y = -abs_height/2; y <= (abs_height/2); y += lineThickness) {
						// For each y, calculate the x coordinates where we intersect the ellipse
						// Using the equation: x = ±(width/2)*sqrt(1 - (y/(height/2))²)
						float term = 1.0 - Math.Pow(y / (abs_height / 2), 2);
						if (term >= 0) {
							float x = (abs_width/2) * Math.Sqrt(term);
							
							// Draw a horizontal line from -x to +x at this y coordinate
							EditorCanvas.DrawLine(center_x - x, center_y + y, center_x + x, center_y + y, lineThickness, 0x644B77BE);
						}
					}

					foreach (EditorMarker marker2: EditorMarker.s_AllMarkers) {
						if (!marker2 || !marker2.GetLayoutRoot().IsVisible()) {
							continue;
						}
						
						EditorObjectMarker object_marker2 = EditorObjectMarker.Cast(marker2);
						
						float x_n2, y_n2;
						marker2.GetLayoutRoot().GetScreenPos(x_n2, y_n2);
						float relative_x = x_n2 - center_x;
						float relative_y = y_n2 - center_y;
						float test = (relative_x * relative_x)/((abs_width/2) * (abs_width/2)) + (relative_y * relative_y)/((abs_height/2) * (abs_height/2));
						if (test <= 1.0) {
							if (object_marker2 && !object_marker2.GetEditorObject().IsSelected()) {
								GetEditor().SelectObject(object_marker2.GetEditorObject());
							}
						} else {
							if (object_marker2 && object_marker2.GetEditorObject().IsSelected()) {
								GetEditor().DeselectObject(object_marker2.GetEditorObject());
							}
						}
					}
					
					break;
				}
			}
		}
		

		bool is_curtain_open = m_TemplateController.LeftbarFrame.IsVisible() || m_TemplateController.RightbarFrame.IsVisible();
		if (input_api.GetInputByID(UAGear).LocalPress()) {
			m_TemplateController.LeftbarFrame.Show(!is_curtain_open);
			m_TemplateController.RightbarFrame.Show(!is_curtain_open);
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
					float LeftWidth = Math.Clamp(mouse_x + wr_col_s_w, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX);
					LeftbarWrapper.SetScreenSize(LeftWidth, wr_s_h);
					GetEditor().GetSettings().LeftBarPlacement = LeftWidth;
					break;
				}

				case RightbarWrapper: {
					RightbarDrag0.SetColor(LinearColor.SLATE_BLUE);
					RightbarCollapsePanel.GetScreenSize(wr_col_s_w, wr_col_s_h);
					RightbarWrapper.GetScreenSize(wr_s_w, wr_s_h);
					float RightWidth = Math.Clamp(screen_x - mouse_x - wr_col_s_w, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX);
					RightbarWrapper.SetScreenSize(Math.Clamp(screen_x - mouse_x - wr_col_s_w, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX), wr_s_h);
					GetEditor().GetSettings().RightBarPlacement = RightWidth;
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

	override bool OnClick(Widget w, int x, int y, int button)
	{
		return super.OnClick(w, x, y, button);
	}
	
	void ToggleObjectSelect()
	{
		m_ObjectSelectToggle = !m_ObjectSelectToggle;
	}

	bool GetObjectSelect()
	{
		return m_ObjectSelectToggle;
	}
	
	void SetSelectionMode(SelectionMode selection_mode)
	{
		m_SelectionMode = selection_mode;
	}
	
	SelectionMode GetSelectionMode()
	{
		return m_SelectionMode;
	}

	override void Show(bool show) 
	{
		super.Show(show);
		
		if (CurrentDialog) {
			CurrentDialog.GetLayoutRoot().Show(show);
		}
	}
	
	void SetEditorMode(eEditorMode editor_mode)
	{
		//@ stub
	}
		
	void ToggleCursor() 
	{	
		// An excellent place to do this!	
		GetEditor().GetSettings().Save();
		
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
		
	void CreateNotification(string text, float duration = 4.0)
	{		
		WidgetAnimator.CancelAnimate(NotificationPanel, WidgetAnimatorProperty.POSITION_Y);
		WidgetAnimator.Animate(NotificationPanel, WidgetAnimatorProperty.POSITION_Y, -24, 100);
		NotificationPanel.SetColor(GetEditor().GetSettings().SelectionColor);
		NotificationText.SetText(text);

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(CleanupNotification);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CleanupNotification, duration * 1000);
	}

	protected void CleanupNotification()
	{
		WidgetAnimator.Animate(NotificationPanel, WidgetAnimatorProperty.POSITION_Y, 0, 100);
	}
	
	bool IsMapVisible()
	{
		return EditorMapWidget.IsVisible();
	}
	
	bool IsSelectionBoxActive()
	{
		return m_IsBoxSelectActive;
	}
	
	void ScrollToListItem(EditorListItem list_item)
	{
		
		//VScrollToWidget(list_item.GetLayoutRoot());
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
	
	bool IsObjectSelectionEnabled()
	{
		return m_ObjectSelectToggle;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/layouts/hud/EditorHud.layout";
	}
	
	override typename GetControllerType()
	{
		return EditorHudController;
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
	
	void ClearCurrentTooltip()
	{
		delete CurrentTooltip;
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
	
	EditorHudController GetTemplateController()
	{
		return m_TemplateController;
	}
	
	static bool IsPointInPolygon(float x, float y, array<vector> points)
	{
		bool inside = false;
        for (int i = 0, j = points.Count() - 1; i < points.Count(); j = i++) {
            if ((points[i][1] > y) != (points[j][1] > y) && x < (points[j][0] - points[i][0]) * (y - points[i][1]) / (points[j][1] - points[i][1]) + points[i][0]) {
                inside = !inside;
            }
        }

        return inside;
	}
}
