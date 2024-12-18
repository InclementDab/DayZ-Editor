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

	protected Editor m_Editor;
	protected bool m_IsBoxSelectActive;
	protected EditorHudController m_TemplateController;
	
	// Layout Elements
	Widget LoggerFrame;

	Widget LeftbarWrapper, RightbarWrapper;
	Widget LeftbarDrag, RightbarDrag;
	Widget LeftbarDrag0, RightbarDrag0;
	ScrollWidget RightbarScroll, LeftbarScroll;
	
	Widget RightbarCollapsePanel, LeftbarCollapsePanel;

	Widget NotificationPanel;
	TextWidget NotificationText;

	protected LinearColor m_MenuColor, m_ToolbarColor;
	protected Widget m_DragWidget;
	protected int m_DragBoxStartX = -1, m_DragBoxStartY = -1;
	protected float m_DragBoxDelayStart;
	protected SelectionMode m_SelectionMode;
	protected bool m_ObjectSelectToggle;
	protected vector m_DragOffset, m_MapPosition;
	protected float m_MapScale = 1.0, m_ScaleActual = 1.0;
	protected float m_ScaleVelocity[1];
	
	Widget Menubar, ToolsWrapper, InfobarFrame, ToolbarFrame;
	Widget LeftbarCategoryConfig, LeftbarCategoryStatic, SearchFavoriteTabPanel;
	Widget CameraPanel;
	Widget LeftbarPanelSearchBarPanel, RightPanelSearchBarPanel, TabPanel;
		
	CanvasWidget EditorCanvas;
	MapWidget Map;
	ImageWidget CameraMarker;
	ButtonWidget CameraPanelButton;
	ButtonWidget CameraTrackPreviousButton, CameraTrackRunButton, CameraTrackNextButton, CameraTrackStopButton, CameraTrackRecordButton; 
	ImageWidget CameraTrackPreviousButton_Icon, CameraTrackRunButton_Icon, CameraTrackNextButton_Icon, CameraTrackStopButton_Icon, CameraTrackRecordButton_Icon;

	EditBoxWidget LeftSearchBar, RightSearchBar;
	Widget LeftSearchBarIcon, RightSearchBarIcon;
	ImageWidget LeftSearchBarIconIcon, RightSearchBarIconIcon;
	TextWidget CameraSpeed;
	
	// Brush info new
	ButtonWidget BrushLeft, BrushRight;
	ImageWidget BrushLeft_Icon, BrushRight_Icon;
	Widget BrushToggle, BrushRadiusFrame, BrushDensityFrame, BrushWidthFrame;
	TextWidget BrushText;
	protected int m_CurrentBrushIndex = 0, m_BrushState = 0;
	protected ref array<ref EditorBrushData> m_BrushTypes = {};

	protected ref array<vector> m_LassoHistory = {};
	
	static const ref array<string> ThemedWidgetStrings = {
		"LeftbarPanelSearchBarIconButton",
		"FavoritesTabButton",
		"ShowPrivateButton",
		"LeftbarCategoryStatic",
		"LeftbarCategoryConfig",
		"DeletionsTabButton",
		"PlacementsTabButton",
		"PlacedSearchIconButton",
		"MenuBarFile",
		"MenuBarEdit",
		"MenuBarView",
		"MenuBarEditor",
		"BrushToggleButton",
		"CinematicCameraButton",
		"CameraTrackMinimizeButton",
		"AddNodeButton",
		"CameraTrackRunButton"
	};

	void EditorHud(notnull Editor editor)
	{	
		m_Editor = editor;
		
		Map.Show(false);
		
		m_TemplateController = EditorHudController.Cast(m_Controller);
		
		m_MenuColor = Menubar.GetColor();
		m_ToolbarColor = ToolsWrapper.GetColor();
		
		// Load Placeable Items
#ifndef COMPONENT_SYSTEM		
		int item_size = m_Editor.GetSettings().ListItemSize;
		array<ref EditorPlaceableItem> placeable_items = m_Editor.GetPlaceableObjects();
		foreach (EditorPlaceableItem placeable_item: placeable_items) {				
			ObservableCollection<ref EditorPlaceableListItem> TargetList;
			// Makes stuff look good when first loading
			switch (placeable_item.Category) {
				case EditorPlaceableItemCategory.CONFIG: {
					TargetList = m_TemplateController.LeftbarSpacerConfig;
					break;
				}
				case EditorPlaceableItemCategory.STATIC: {
					TargetList = m_TemplateController.LeftbarSpacerStatic;
					break;
				}
				//? fall-through removed 
				case EditorPlaceableItemCategory.SCRIPTED: {
					TargetList = m_TemplateController.LeftbarSpacerStatic;
					break;
				}
			}
			
			EditorPlaceableListItem list_item;
			switch (item_size) {
				case 2: {
					list_item = new EditorPlaceableListItemLarge(placeable_item);
					break;
				}
				
				case 1:
				default: {
					list_item = new EditorPlaceableListItem(placeable_item);
					break;
				}	
			}
			if (placeable_item.IsFavorite()) {
				TargetList.InsertAt(list_item, 0);
			} else {
				TargetList.Insert(list_item);
			}
			
			bool gay = (placeable_item.Scope > 0 || m_TemplateController.ShowPrivate);
			list_item.Show(gay);
		}
		
		EditorLog.Info("Loaded %1 Placeable Objects", placeable_items.Count().ToString());
		
		SearchFavoriteTabPanel.SetColor(m_ToolbarColor);
		
		foreach (string themed_widget_name: ThemedWidgetStrings) {
			Widget themed_widget = m_LayoutRoot.FindAnyWidget(themed_widget_name);
			if (themed_widget) {
				themed_widget.SetColor(m_Editor.GetSettings().SelectionColor);
			}
		}

		// Load Brushes
		ReloadBrushes(m_Editor.GetSettings().BrushFile);
#endif		
	
		m_TemplateController.ShowPrivate = m_Editor.GetSettings().ShowScopeZeroObjects;
		m_TemplateController.NotifyPropertyChanged("ShowPrivate");

		m_TemplateController.FavoritesToggle = m_Editor.GetSettings().ShowFavoriteObjects;
		m_TemplateController.NotifyPropertyChanged("FavoritesToggle");
		
		EditorCamera camera = m_Editor.GetCamera();
		m_TemplateController.CameraControls.Insert(new SliderPrefab("FOV", camera, "FOV", EditorCamera.FOV_MIN * Math.RAD2DEG, EditorCamera.FOV_MAX * Math.RAD2DEG));
		m_TemplateController.CameraControls.Insert(new SliderPrefab("View Distance", GetEditor().GetCameraSettings(), "ViewDistance", EditorCamera.VIEW_DISTANCE_MIN, EditorCamera.VIEW_DISTANCE_MAX));
		m_TemplateController.CameraControls.Insert(new SliderPrefab("Gaussian Blur", camera, "Blur", 0, 1));
		m_TemplateController.CameraControls.Insert(new SliderPrefab("Near Plane", camera, "NearPlane",  0, 1));
		m_TemplateController.CameraControls.Insert(new SliderPrefab("DOF Distance", camera, "DOFDistance", 0, 500));
		m_TemplateController.CameraControls.Insert(new SliderPrefab("DOF Blur", camera, "DOFBlur", 0, 1));
		m_TemplateController.CameraControls.Insert(new SliderPrefab("Vignette", camera, "Vignette", 0, 1));
		m_TemplateController.CameraControls.Insert(new SliderPrefab("Sharpness", camera, "Sharpness", 0, 1));
		m_TemplateController.CameraControls.Insert(new SliderPrefab("Exposure", camera, "Exposure", 0, 3));
		m_TemplateController.CameraControls.Insert(new SliderPrefab("Smoothing", camera, "Smoothing", 0, 1));
						
		ShowScreenLogs(m_Editor.GetSettings().ShowScreenLogs);
		
		// Set up toolbars based on screen size
		int screen_w, screen_h;
		GetScreenSize(screen_w, screen_h);
		
		float tb_s_w, tb_s_h;
		ToolbarFrame.GetScreenSize(tb_s_w, tb_s_h);
		
		float ib_s_w, ib_s_h;
		InfobarFrame.GetScreenSize(ib_s_w, ib_s_h);
		
		float lbw_s_w, lbw_s_h;
		float rbw_s_w, rbw_s_h;
		LeftbarWrapper.GetScreenSize(lbw_s_w, lbw_s_h);
		RightbarWrapper.GetScreenSize(rbw_s_w, rbw_s_h);
		
		float bar_height = screen_h - ib_s_h - tb_s_h;
		LeftbarWrapper.SetScreenSize(m_Editor.GetSettings().LeftBarPlacement, bar_height);
		RightbarWrapper.SetScreenSize(m_Editor.GetSettings().RightBarPlacement, bar_height);
		
		// Leftbar scroll size
		float lbs_s_w, lbs_s_h;
		float lpsbp_s_w, lpsbp_s_h, sftp_s_w, sftp_s_h;
		LeftbarPanelSearchBarPanel.GetScreenSize(lpsbp_s_w, lpsbp_s_h);
		SearchFavoriteTabPanel.GetScreenSize(sftp_s_w, sftp_s_h);
		LeftbarScroll.GetScreenSize(lbs_s_w, lbs_s_h);
		LeftbarScroll.SetScreenSize(lbs_s_w, bar_height - sftp_s_h - lpsbp_s_h);
		
		// Rightbar scroll size
		float tp_s_w, tp_s_h, rpsbp_s_h, rpsbp_s_w, rbs_s_w, rbs_s_h;
		TabPanel.GetScreenSize(tp_s_w, tp_s_h);
		RightPanelSearchBarPanel.GetScreenSize(rpsbp_s_w, rpsbp_s_h);
		RightbarScroll.GetScreenSize(rbs_s_w, rbs_s_h);
		RightbarScroll.SetScreenSize(rbs_s_w, bar_height - tp_s_h - rpsbp_s_h);
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		if (!GetGame().IsAppActive()) {
			return;
		}
		
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);

		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);

		UAInputAPI input_api = GetUApi();
		UAInput left_mouse_input = input_api.GetInputByID(UAMenuSelect);
		UAInput right_mouse_input = input_api.GetInputByID(UAMenuBack);
		UAInput toggle_hud_input = input_api.GetInputByName("EditorToggleUI");
		UAInput toggle_cursor = input_api.GetInputByName("EditorToggleCursor");
		UAInput toggle_editor = input_api.GetInputByName("EditorToggleActive");
		UAInput teleport_to_cursor = input_api.GetInputByName("EditorTeleportPlayerToCursor");
		UAInput toggle_map = input_api.GetInputByName("EditorToggleMap");
		UAInput zoom_up = input_api.GetInputByID(UAZoomInOptics);
		UAInput zoom_down = input_api.GetInputByID(UAZoomOutOptics);
		UAInput shift_input = input_api.GetInputByID(UATurbo);
		
		Widget widget_under_cursor = GetWidgetUnderCursor();
		Widget focus_widget = GetFocus();
		bool cursor_visible = GetGame().GetUIManager().IsCursorVisible();
		bool input_unlocked = (!focus_widget || !focus_widget.IsInherited(EditBoxWidget)) && !m_Dialog;
		bool any_mouse_press = (left_mouse_input.LocalPress() || right_mouse_input.LocalPress());

		// Clear focus!
		if (any_mouse_press && !widget_under_cursor) {
			SetFocus(null);
			delete CurrentMenu;			
			SetCurrentTooltip(null);
		}

		if (m_Editor.IsInventoryEditorActive()) {
			Show(false);
			return;
		}
		
		if (toggle_map.LocalPress() && input_unlocked) {
			Map.Show(!Map.IsVisible());
			Map.SetMapPos(GetGame().GetCurrentCameraPosition());
			ShowCursor(true);
		
			EditorEvents.MapToggled(this, Map, Map.IsVisible());
			return;
		}
		
		if (Map.IsVisible()) {
			UpdateMap(Map, dt);
		}
				
#ifdef GIZMOS_ENABLED
		
		// todo: a cursor hide mask
		//if (m_Editor.GetGizmo() && m_Editor.GetGizmo().IsInteracting()) {
		//	ShowCursor(false);
		//	return;
		//}
#endif
		
		// lctrl for commands
		if (toggle_editor.LocalPress() && !GetDayZGame().IsLeftCtrlDown() && input_unlocked) {
			// Control current player
			if (m_Editor.IsActive()) {
				m_Editor.ControlPlayer(m_Editor.GetPlayer());
			} else {
				m_Editor.Activate();
			}
		}
		
		if (toggle_hud_input.LocalPress() && input_unlocked && !GetDayZGame().IsLeftCtrlDown()) {		
			Show(!IsVisible());
		}
		
		// Dont want to toggle cursor on map 
		if (toggle_cursor.LocalPress() && input_unlocked) {
			if (!Map.IsVisible() && !m_Editor.IsPlayerControlled() && m_Editor.IsActive() && !(m_Dialog && EditorHud.CurrentDialog && m_Editor.GetSettings().LockCameraDuringDialogs)) {	
				ToggleCursor();
			}
		}
		
		// Teleport the player
		if (teleport_to_cursor.LocalPress() && GetGame().GetUIManager().IsCursorVisible() && !GetDayZGame().IsLeftCtrlDown()) {
			PlayerBase teleport_player = m_Editor.GetControllingPlayer();
			if (!teleport_player) {
				teleport_player = m_Editor.GetPlayer();
			}
			
			if (teleport_player) {
				Raycast teleport_player_raycast = m_Editor.GetCursorRaycastModeSafe(teleport_player);
				if (teleport_player_raycast) {
					teleport_player.SetPosition(teleport_player_raycast.Bounce.Position);
				}
			}
		}

		if (left_mouse_input.LocalPress() && m_DragBoxStartX == -1 && m_DragBoxStartY == -1) {
			if ((!widget_under_cursor || widget_under_cursor == Map) && GetGame().GetInput().HasGameFocus() && cursor_visible && !m_Editor.IsPlacing() && !m_Editor.IsDragging()) {
				m_DragBoxDelayStart = 0.12;
				GetMousePos(m_DragBoxStartX, m_DragBoxStartY);
				m_LassoHistory.Clear();
			}

			Map.SetFlags(WidgetFlags.IGNOREPOINTER);
		}
		
		if (left_mouse_input.LocalRelease()) {
			m_DragWidget = null;
			m_DragBoxDelayStart = 10;
			m_DragBoxStartX = -1;
			m_DragBoxStartY = -1;
			Map.ClearFlags(WidgetFlags.IGNOREPOINTER);
		}

		float rs_s_w, rs_s_h;
		RightbarScroll.GetScreenSize(rs_s_w, rs_s_h);
		if (RightbarScroll.GetVScrollPos() + rs_s_h > RightbarScroll.GetContentHeight()) {
			RightbarScroll.VScrollToPos(RightbarScroll.GetContentHeight());
		}

		float ls_s_w, ls_s_h;
		LeftbarScroll.GetScreenSize(ls_s_w, ls_s_h);
		if (LeftbarScroll.GetVScrollPos() + ls_s_h > LeftbarScroll.GetContentHeight()) {
			LeftbarScroll.VScrollToPos(LeftbarScroll.GetContentHeight());
		}
				
		EditorCanvas.Clear();
		m_DragBoxDelayStart -= dt;
		if (left_mouse_input.LocalValue() && m_DragBoxDelayStart < 0 && GetGame().GetInput().HasGameFocus() && cursor_visible && !m_Editor.IsPlacing() && !m_Editor.IsDragging() && !m_Editor.Brush && !m_DragWidget && m_DragBoxStartX != -1 && m_DragBoxStartY != -1 && EditorMarker.s_AllMarkers) {	
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
								m_Editor.SelectObject(object_marker0.GetEditorObject());
							}
						} else {
							if (object_marker0 && object_marker0.GetEditorObject().IsSelected() && !shift_input.LocalValue()) {
								m_Editor.DeselectObject(object_marker0.GetEditorObject());
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
						if (!marker || !marker.GetLayoutRoot() || !marker.GetLayoutRoot().IsVisible()) {
							continue;
						}
						
						EditorObjectMarker object_marker = EditorObjectMarker.Cast(marker);
						
						float x_n, y_n;
						marker.GetLayoutRoot().GetScreenPos(x_n, y_n);
						//if (top_left[0] <= m_screen_x && m_screen_x <= bottom_right[0] && top_left[1] <= m_screen_y && m_screen_y <= bottom_right[1]) {
						if ((x_n < Math.Max(m_DragBoxStartX, mouse_x) && x_n > Math.Min(m_DragBoxStartX, mouse_x)) && (y_n < Math.Max(m_DragBoxStartY, mouse_y) && y_n > Math.Min(m_DragBoxStartY, mouse_y))) {
							if (object_marker && !object_marker.GetEditorObject().IsSelected()) {
								m_Editor.SelectObject(object_marker.GetEditorObject());
							}
						} else {
							if (object_marker && object_marker.GetEditorObject().IsSelected() && !shift_input.LocalValue()) {
								m_Editor.DeselectObject(object_marker.GetEditorObject());
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
								m_Editor.SelectObject(object_marker2.GetEditorObject());
							}
						} else {
							if (object_marker2 && object_marker2.GetEditorObject().IsSelected() && !shift_input.LocalValue()) {
								m_Editor.DeselectObject(object_marker2.GetEditorObject());
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
				if (left_mouse_input.LocalPress()) {
					m_DragWidget = LeftbarWrapper;
				}
				
				if (left_mouse_input.LocalDoubleClick()) {
					LeftbarWrapper.GetSize(wr_s_w, wr_s_h);
					LeftbarWrapper.SetSize(DEFAULT_BAR_WIDTH_PX, wr_s_h);
				}
				
				break;
			}

			case RightbarDrag: {
				if (left_mouse_input.LocalPress()) {
					m_DragWidget = RightbarWrapper;
				}

				if (left_mouse_input.LocalDoubleClick()) {
					RightbarWrapper.GetSize(wr_s_w, wr_s_h);
					RightbarWrapper.SetSize(DEFAULT_BAR_WIDTH_PX, wr_s_h);
				}

				break;
			}
		}
		
		if (m_DragWidget) {
			switch (m_DragWidget) {
				case LeftbarWrapper: {
					LeftbarDrag0.SetColor(LinearColor.SLATE_BLUE);
					LeftbarCollapsePanel.GetScreenSize(wr_col_s_w, wr_col_s_h);
					LeftbarWrapper.GetScreenSize(wr_s_w, wr_s_h);
					float LeftWidth = Math.Clamp(mouse_x + wr_col_s_w + 25, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX);
					LeftbarWrapper.SetScreenSize(LeftWidth, wr_s_h);
					m_Editor.GetSettings().LeftBarPlacement = LeftWidth;
					break;
				}

				case RightbarWrapper: {
					RightbarDrag0.SetColor(LinearColor.SLATE_BLUE);
					RightbarCollapsePanel.GetScreenSize(wr_col_s_w, wr_col_s_h);
					RightbarWrapper.GetScreenSize(wr_s_w, wr_s_h);
					float RightWidth = Math.Clamp(screen_x - mouse_x + wr_col_s_w + 25, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX);
					RightbarWrapper.SetScreenSize(RightWidth, wr_s_h);
					m_Editor.GetSettings().RightBarPlacement = RightWidth;
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

		if (m_Editor.IsRunningCameraTrack()) {
			Symbols.PAUSE.Load(CameraTrackRunButton_Icon);
		} else {
			Symbols.PLAY.Load(CameraTrackRunButton_Icon);
		}
		
		int color = Ternary<int>.If(m_BrushState, m_Editor.GetSettings().SelectionColor, 0xff262729);
		if (widget_under_cursor != BrushToggle) {
			BrushToggle.SetColor(color);
		}
		
		string speed_s = Math.Round(GetEditor().GetCamera().GetSettings().Speed).ToString();
		CameraSpeed.SetText(speed_s);
		
#ifdef DIAG_DEVELOPER
		float tbf_s_w, tbf_s_h;
		ToolbarFrame.GetScreenSize(tbf_s_w, tbf_s_h);
		DbgUI.Begin("Editor", m_Editor.GetSettings().LeftBarPlacement + 24, tbf_s_h + 24);
		string widget_under_cursor_name = "None";
		string focus_widget_name = "None";
		if (widget_under_cursor) {
			widget_under_cursor_name = widget_under_cursor.GetName();
		}
		if (focus_widget) {
			focus_widget_name = focus_widget.GetName();
		}

		DbgUI.Text(string.Format("Widget Under Cursor: %1", widget_under_cursor_name));
		DbgUI.Text(string.Format("Focus Widget: %1", focus_widget_name));
		DbgUI.End();
#endif
	}
		
	protected void UpdateMap(notnull MapWidget map_widget, float dt)
	{
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);
		UAInputAPI input = GetUApi();
		UAInput left_mouse_input = input.GetInputByID(UAMenuSelect);
		UAInput right_mouse_input =  input.GetInputByID(UAMenuBack);
		UAInput speed_modifier_input =  input.GetInputByID(UATurbo);
		int world_size = GetGame().GetWorld().GetWorldSize();
		
		vector mouse_world = map_widget.ScreenToMap(Vector(mouse_x, mouse_y, 0));
		
		float scale_min = GetGame().ConfigGetFloat("RscMapControl scaleMin");
		float scale_max = GetGame().ConfigGetFloat("RscMapControl scaleMax");
		float scale_01 = Math.InverseLerp(scale_min, scale_max, m_MapScale);
		
		float scale_change_value = Math.Exp(scale_01) / (50 - 30 * speed_modifier_input.LocalValue());
		if (input.GetInputByID(UANextAction).LocalValue()) {
			m_MapScale += scale_change_value;
		}
		
		if (input.GetInputByID(UAPrevAction).LocalValue()) {
			m_MapScale -= scale_change_value;
		}
		
		m_MapScale = Math.Clamp(m_MapScale, scale_min, scale_max);
		
		float vertical_view_size = m_MapScale * world_size;		
		if (input.GetInputByID(UAUIRotateInventory).LocalPress()) {
			m_MapPosition = GetGame().GetCurrentCameraPosition();
			m_MapScale = 0.333;
		}
		
		if (right_mouse_input.LocalPress()) {
			SetFocus(null);
			m_DragOffset = map_widget.ScreenToMap(Vector(mouse_x, mouse_y, 0));
		}			
		
		float p[1];
		copyarray(p, m_ScaleVelocity);		
				
		if (!right_mouse_input.LocalValue()) {
			//Scale = Math.Clamp(Scale, 0.05, 0.95);
			
			//m_ScaleActual += (Scale - m_SclaeActual) * dt;
			
			m_ScaleActual = Math.SmoothCD(m_ScaleActual, m_MapScale, p, 0.007, 100, dt);
			//Scale = Math.Clamp(m_ScaleActual, 0.05, 0.95);
		} else {
			m_ScaleActual = m_MapScale;
		}

		map_widget.SetScale(m_ScaleActual);
		
		if (right_mouse_input.LocalValue()) {
			m_MapPosition = m_DragOffset - map_widget.ScreenToMap(Vector(mouse_x, mouse_y, 0)) + map_widget.GetMapPos();
		}
		
		map_widget.SetMapPos(m_MapPosition);
		
		// Camera yaw matrix
		vector map_to_screen_cam = map_widget.MapToScreen(GetGame().GetCurrentCameraPosition());
		float c_s_x, c_s_y;
		CameraMarker.GetScreenSize(c_s_x, c_s_y);
		CameraMarker.SetScreenPos(map_to_screen_cam[0] - c_s_x / 2, map_to_screen_cam[1] - c_s_y / 2);
		
		float camera_yaw = GetGame().GetCurrentCameraDirection().VectorToAngles()[0];
		CameraMarker.SetRotation(0, 0, camera_yaw - 90);		
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{		
		switch (w) {
			case CameraPanelButton: {
				CameraPanel.Show(!CameraPanel.IsVisible());
				break;
			}

			case CameraTrackPreviousButton: {

				break;
			}

			case CameraTrackRunButton: {
				if (m_Editor.IsRunningCameraTrack()) {
					m_Editor.PauseCameraTrack();
				} else {
					m_Editor.StartCameraTrack();
				}

				break;
			}

			case CameraTrackNextButton: {

				break;
			}

			case CameraTrackStopButton: {
				m_Editor.StopCameraTrack();
				break;
			}

			case CameraTrackRecordButton: {

				break;
			}
			
			case BrushRight: {
				SetBrushIndex(Math.Rollover(m_CurrentBrushIndex + 1, 0, m_BrushTypes.Count()));
				break;
			}
			
			case BrushLeft: {
				SetBrushIndex(Math.Rollover(m_CurrentBrushIndex - 1, 0, m_BrushTypes.Count()));
				break;
			}
		}

		return super.OnClick(w, x, y, button);
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return super.OnMouseButtonUp(w, x, y, button);
		}

		switch (w) {
			case BrushToggle: {
				SetBrushState(!GetBrushState());
				break;
			}
		}

		return super.OnMouseButtonUp(w, x, y, button);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (w) {
			case LeftSearchBar: {
				if (button == 1) {
					LeftSearchBar.SetText(string.Empty);
					OnChange(LeftSearchBar, x, y, true);
					return true;
				}
				
				break;
			}
			
			case RightSearchBar: {
				if (button == 1) {
					RightSearchBar.SetText(string.Empty);
					OnChange(RightSearchBar, x, y, true);
					return true;
				}
				break;
			}
			
			case LeftSearchBarIcon: {
				LeftSearchBar.SetText(string.Empty);
				OnChange(LeftSearchBar, x, y, true);
				break;
			}
			
			case RightSearchBarIcon: {
				RightSearchBar.SetText(string.Empty);
				OnChange(RightSearchBar, x, y, true);
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case BrushRight: {
				BrushRight_Icon.SetImage(2);
				break;
			}
			
			case BrushLeft: {
				BrushLeft_Icon.SetImage(2);
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case LeftbarCategoryStatic: {
				CreateDelayedTooltip(w, "Static Objects", TooltipPosition.TOP_LEFT, "Static Non-Interactive Objects");
				break;
			}

			case LeftbarCategoryConfig: {
				CreateDelayedTooltip(w, "Config Objects", TooltipPosition.TOP_LEFT, "Interactive Objects & Items");
				break;
			}
			
			case BrushToggle: {
				if (!m_BrushState) {
					LinearColor c = m_Editor.GetSettings().SelectionColor;
					
					WidgetAnimator.AnimateColor(w, c.With(3, 100), 100);
				}
				
				break;
			}
			
			case BrushRight: {
				BrushRight_Icon.SetImage(3);
				break;
			}
			
			case BrushLeft: {
				BrushLeft_Icon.SetImage(3);
				break;
			}
			
			case RightSearchBar:
			case LeftSearchBar: {
				return true;
			}
		}

		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		switch (w) {
			case LeftSearchBar:
			case RightSearchBar: {
				m_Editor.CancelPlacing();
				break;
			}
		}

		return super.OnFocus(w, x, y);
	}	
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		switch (w) {
			case LeftSearchBar: {
				string left_search_bar_text = LeftSearchBar.GetText();
				array<ObservableCollection<ref EditorPlaceableListItem>> collections = { m_TemplateController.LeftbarSpacerConfig, m_TemplateController.LeftbarSpacerStatic };
				foreach (auto collection: collections) {
					for (int j = 0; j < collection.Count(); j++) {
						int hide = !collection[j].FilterType(left_search_bar_text);
						if (m_TemplateController.FavoritesToggle) {
							hide |= hide | (!collection[j].GetTemplateController().Favorite << 1);
						}

						if (!m_TemplateController.ShowPrivate) {
							hide |= hide | (collection[j].GetPlaceableItem().Scope < 2) << 2;
						}

						collection[j].GetLayoutRoot().Show(!hide);
					}
				}
				
				LeftbarScroll.VScrollToPos(0);
				
				Symbols left_search_bar_icon = Ternary<Symbols>.If(!left_search_bar_text.Length(), Symbols.MAGNIFYING_GLASS, Symbols.X);
				left_search_bar_icon.Load(LeftSearchBarIconIcon);
				break;
			}
			
			case RightSearchBar: {
				string right_search_bar_text = RightSearchBar.GetText();
				auto right_spacer_config = Ternary<ObservableCollection<EditorListItem>>.If(m_TemplateController.CategoryPlacements, m_TemplateController.RightbarPlacedData, m_TemplateController.RightbarDeletionData);
				for (int i = 0; i < right_spacer_config.Count(); i++) {					
					right_spacer_config[i].GetLayoutRoot().Show(right_spacer_config[i].FilterType(right_search_bar_text));
				}
				
				RightbarScroll.VScrollToPos(0);
				Symbols right_search_bar_icon = Ternary<Symbols>.If(!right_search_bar_text.Length(), Symbols.MAGNIFYING_GLASS, Symbols.X);
				right_search_bar_icon.Load(RightSearchBarIconIcon);
				break;
			}
		}
		
		return super.OnChange(w, x, y, finished);
	}
		
	void SetObjectSelectState(bool state)
	{
		m_ObjectSelectToggle = state;
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
		if (m_LayoutRoot.IsVisible() == show) {
			return;
		}
		
		super.Show(show);
		
		if (CurrentDialog) {
			CurrentDialog.GetLayoutRoot().Show(show);
		}
		
		if (m_Dialog) {
			m_Dialog.GetLayoutRoot().Show(show);
		}
		
		GetGame().GetUIManager().ShowCursor(show);
	}
		
	void SetEditorMode(eEditorMode editor_mode)
	{
		//@ stub
	}
		
	void ToggleCursor() 
	{	
		// An excellent place to do this!	
		m_Editor.GetSettings().Save();
		m_Editor.GetCameraSettings().Save();
		
		ShowCursor(!GetGame().GetUIManager().IsCursorVisible());
	}
	
	void ShowCursor(bool state) 
	{
		GetGame().GetUIManager().ShowCursor(state);
		
		if (!state) {
			delete CurrentMenu;
			SetFocus(null);
			ClearCurrentTooltip();
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
		NotificationPanel.SetColor(m_Editor.GetSettings().SelectionColor);
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
		return Map.IsVisible();
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
	void SetCurrentTooltip(ScriptView current_tooltip) 
	{
		if (!current_tooltip) {
			ClearCurrentTooltip();
		} else {
			if (CanCreateTooltip()) {
				GetDayZGame().SetCurrentTooltip(current_tooltip);
			}
		}
	}
	
	TooltipView CreateDelayedTooltip(Widget w, string text, TooltipPosition position, string desc = string.Empty, Symbols icon = string.Empty, int delay = 300)
	{
		return GetDayZGame().CreateDelayedTooltip(w, text, position, desc, icon, delay);
	}

	protected bool CanCreateTooltip()
	{
		return !IsSelectionBoxActive();
	}
	
	void ClearCurrentTooltip()
	{
		GetDayZGame().ClearTooltip();
	}
	
	bool ReloadBrushes(string file)
	{
		string brushes_filename = SystemPath.Format(file);
		if (File.Exists(brushes_filename)) {
			XMLEditorBrushes xml_brushes = new XMLEditorBrushes(m_BrushTypes);
			GetXMLApi().Read(brushes_filename, xml_brushes);
			SetBrushIndex(0);
			
			float largest_size;
			for (int i = 0; i < m_BrushTypes.Count(); i++) {
				float size = m_BrushTypes[i].Name.Length() * 12;
				if (size > largest_size) {
					largest_size = size;
				}
			}
			
			float bt_s_w, bt_s_h;
			BrushToggle.GetScreenSize(bt_s_w, bt_s_h);
			BrushToggle.SetScreenSize(largest_size, bt_s_h);
			return true;
		}

		return false;
	}

	void SetBrushState(int state)
	{
		m_BrushState = state;
		
		EditorBrushData brush_data = m_BrushTypes[m_CurrentBrushIndex];
		if (brush_data && m_BrushState) {
			m_Editor.Brush = EditorBrush.Create(brush_data);
		} else {
			m_Editor.Brush = null;
		}

		BrushRadiusFrame.Show(m_BrushState);
		BrushDensityFrame.Show(m_BrushState);
		BrushWidthFrame.Show(m_BrushState);
	}

	int GetBrushState()
	{
		return m_BrushState;
	}
	
	void SetBrushIndex(int index)
	{
		if (!m_BrushTypes.IsValidIndex(index)) {
			return;
		}
		
		m_CurrentBrushIndex = index;
		EditorBrushData brush_data = m_BrushTypes[m_CurrentBrushIndex];
		string name = m_BrushTypes[m_CurrentBrushIndex].Name;
		BrushText.SetText(name);
		
		if (m_BrushState) {
			m_Editor.Brush = EditorBrush.Create(brush_data);
		}
	}

	int GetBrushIndex()
	{
		return m_CurrentBrushIndex;
	}
	
	void SetBrushByTypename(typename type)
	{
		for (int i = 0; i < m_BrushTypes.Count(); i++) {
			if (m_BrushTypes[i].BrushClassName == type) {
				SetBrushIndex(i);
				return;
			}
		}		
	}
	
	void SetBrushByName(string name)
	{
		for (int i = 0; i < m_BrushTypes.Count(); i++) {
			if (m_BrushTypes[i].Name == name || m_BrushTypes[i].BrushClassName.ToString() == name) {
				SetBrushIndex(i);
				return;
			}
		}	
	}
		
	ScriptView ShowFileDialog(string title, typename file_type, ScriptCaller on_file_chosen, eDialogMode dialog_mode, eDialogFlags dialog_flags = 0)
	{
		EditorFileDialog dialog = new EditorFileDialog(file_type, on_file_chosen, dialog_mode, dialog_flags);
		m_Dialog = dialog;
		
		GetGame().GetUIManager().ShowCursor(true);
		return m_Dialog;
	}
	
	ScriptView ShowMessageBox(string caption, MessageBoxButtons buttons, ScriptCaller on_close)
	{
		EditorMessageBox message_box = new EditorMessageBox(caption, buttons, on_close);
		m_Dialog = message_box;
		GetGame().GetUIManager().ShowCursor(true);
		return m_Dialog;
	}
	
	ScriptView GetDialog()
	{
		return m_Dialog;
	}
	
	protected ref ScriptView m_Dialog;
	protected ref ScriptView m_MessageBox;
		
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
