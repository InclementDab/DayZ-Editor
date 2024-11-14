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
	
	CanvasWidget EditorCanvas;
	
	ref EditorCameraMapMarker CameraMapMarker;
	
	// todo protect this and move all Map logic in here?
	MapWidget EditorMapWidget;
	
	EditBoxWidget LeftbarSearchBar;
	
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
				g_Editor.ClearSelection();
			}

			EditorMapWidget.SetFlags(WidgetFlags.IGNOREPOINTER);
		}
		
		if (input.LocalRelease("UAFire")) {
			m_DragBoxDelayStart = -5;
			EditorMapWidget.ClearFlags(WidgetFlags.IGNOREPOINTER);
		}
		
		EditorCanvas.Clear();
		if (input.LocalValue("UAFire") && m_DragBoxDelayStart < 0 && m_DragBoxDelayStart > -1 && GetGame().GetInput().HasGameFocus() && cursor_visible && !GetEditor().IsPlacing() && !GetEditor().IsDragging()) {
			LinearColor drag_box_color = 0xff0078D4;
			LinearColor drag_box_color_fill = drag_box_color.With(3, 60);
			int current_x, current_y;
			GetMousePos(current_x, current_y);
			// @Sumrak :ANGERY:
			//current_x += 6;
			
			// Draw Drag Box
			EditorCanvas.DrawLine(m_DragBoxStartX, m_DragBoxStartY, current_x, m_DragBoxStartY, DRAG_BOX_THICKNESS, drag_box_color);
			EditorCanvas.DrawLine(m_DragBoxStartX, m_DragBoxStartY, m_DragBoxStartX, current_y, DRAG_BOX_THICKNESS, drag_box_color);
			EditorCanvas.DrawLine(m_DragBoxStartX, current_y, current_x, current_y, DRAG_BOX_THICKNESS, drag_box_color);
			EditorCanvas.DrawLine(current_x, m_DragBoxStartY, current_x, current_y, DRAG_BOX_THICKNESS, drag_box_color);


			vector top_left = Vector(Math.Min(m_DragBoxStartX, current_x), Math.Min(m_DragBoxStartY, current_y), 0);
			vector bottom_right = Vector(Math.Max(m_DragBoxStartX, current_x), Math.Max(m_DragBoxStartY, current_y), 0);
			
			// Handles the fill operation
			int x_avg = (m_DragBoxStartX + current_x) / 2;
			EditorCanvas.DrawLine(x_avg, m_DragBoxStartY, x_avg, current_y, current_x - m_DragBoxStartX, drag_box_color_fill); 
			
			foreach (EditorMarker marker: EditorMarker.s_AllMarkers) {
				if (!marker || !marker.GetLayoutRoot().IsVisible()) {
					continue;
				}
				
				EditorObjectMarker object_marker = EditorObjectMarker.Cast(marker);
				
				float m_screen_x, m_screen_y;
				marker.GetLayoutRoot().GetScreenPos(m_screen_x, m_screen_y);
				if (top_left[0] <= m_screen_x && m_screen_x <= bottom_right[0] && top_left[1] <= m_screen_y && m_screen_y <= bottom_right[1]) {
					if (object_marker && !object_marker.GetEditorObject().IsSelected()) {
						GetEditor().SelectObject(object_marker.GetEditorObject());
					}
				} else {
					if (object_marker && object_marker.GetEditorObject().IsSelected()) {
						GetEditor().DeselectObject(object_marker.GetEditorObject());
					}
				}
			}
			
		} else {
			m_DragBoxDelayStart -= dt;
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
}
