enum SelectionMode
{
	BOX,
	ELLIPSE,
	LASSO // ???? are you CRAZY?
};

class EditorHud: ScriptViewMenu
{
	const float DEFAULT_BAR_WIDTH_PX = 380.0;

	const float BAR_WIDTH_MINIMUM_PX = 100.0;
	const float BAR_WIDTH_MAXIMUM_PX = 900.0;

	protected Editor m_Editor;
	protected bool m_IsBoxSelectActive;
	protected EditorHudController m_TemplateController;
	
	// Layout Elements
	Widget NotificationFrame, MapContainer, LoggerFrame;

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
	
	Widget Menubar, ToolsWrapper;
	Widget PlacementsTabButton, DeletionsTabButton, LeftbarCategoryConfig, LeftbarCategoryStatic, SearchFavoriteTabPanel;
		
	CanvasWidget EditorCanvas;
	
	RTTextureWidget SelectionTextures;
	
	ref EditorCameraMapMarker CameraMapMarker;
	
	// todo protect this and move all Map logic in here?
	MapWidget EditorMapWidget;
	
	EditBoxWidget LeftSearchBar, RightSearchBar;
	Widget LeftSearchBarIcon, RightSearchBarIcon;
	ImageWidget LeftSearchBarIconIcon, RightSearchBarIconIcon;

	protected ref array<vector> m_LassoHistory = {};
	
	static const ref array<string> ThemedWidgetStrings = {
		"GizmoTranslateButton",
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
		
		EditorMapWidget.Show(false);
		
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
		
		string brush_file = SystemPath.Format(m_Editor.GetSettings().BrushFile);		
		if (!FileExist(brush_file) && !CopyFile("DayZEditor\\scripts\\data\\Defaults\\Brushes.xml", brush_file)) {
			Error(string.Format("Could not copy brush data to %1", brush_file));
		} else ReloadBrushes(brush_file);
		
#endif		

		m_TemplateController.ShowPrivate = m_Editor.GetSettings().ShowScopeZeroObjects;
		m_TemplateController.NotifyPropertyChanged("ShowPrivate");

		m_TemplateController.FavoritesToggle = m_Editor.GetSettings().ShowFavoriteObjects;
		m_TemplateController.NotifyPropertyChanged("FavoritesToggle");
						
		ShowScreenLogs(m_Editor.GetSettings().ShowScreenLogs);
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		float s_r_w, s_r_h, s_l_w, s_l_h;
		RightbarWrapper.GetScreenSize(s_r_w, s_r_h);
		RightbarWrapper.SetScreenSize(m_Editor.GetSettings().RightBarPlacement, s_r_h);
		LeftbarWrapper.GetScreenSize(s_l_w, s_l_h);
		LeftbarWrapper.SetScreenSize(m_Editor.GetSettings().LeftBarPlacement, s_l_h);
	}
	
	int ReloadBrushes(string filename)
	{
		filename = SystemPath.Format(filename);
		if (!File.Exists(filename)) {
			PrintFormat("file not found %1", filename);
			return 0;
		}
		
		m_TemplateController.BrushToggleButtonState = false;
		m_TemplateController.NotifyPropertyChanged("BrushToggleButtonState");
		
		m_TemplateController.BrushTypeBoxData.Clear();
		XMLEditorBrushes xml_brushes = new XMLEditorBrushes(m_TemplateController.BrushTypeBoxData);
		GetXMLApi().Read(filename, xml_brushes);
		return m_TemplateController.BrushTypeBoxData.Count();
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
		
		UAInput click_input = input_api.GetInputByID(UAFire);
		UAInput toggle_hud_input = input_api.GetInputByName("EditorToggleUI");
		UAInput toggle_cursor = input_api.GetInputByName("EditorToggleCursor");
		UAInput toggle_editor = input_api.GetInputByName("EditorToggleActive");
		UAInput teleport_to_cursor = input_api.GetInputByName("EditorTeleportPlayerToCursor");
		
		Widget widget_under_cursor = GetWidgetUnderCursor();
		bool cursor_visible = GetGame().GetUIManager().IsCursorVisible();
		
		if (m_Editor.IsInventoryEditorActive()) {
			Show(false);
			return;
		}
		
		// lctrl for commands
		if (toggle_editor.LocalPress() && !GetDayZGame().IsLeftCtrlDown()) {
			// Control current player
			if (m_Editor.IsActive()) {
				m_Editor.ControlPlayer(m_Editor.GetPlayer());
			} else {
				m_Editor.Activate();
			}
		}
		
		if (toggle_hud_input.LocalPress() && (!GetFocus() || !GetFocus().IsInherited(EditBoxWidget))) {		
			Show(!IsVisible());
		}
		
		// Dont want to toggle cursor on map
		if (toggle_cursor.LocalPress()) {
			if (!EditorMapWidget.IsVisible() && !GetEditor().IsPlayerControlled() && GetEditor().IsActive() && !(EditorHud.CurrentDialog && m_Editor.GetSettings().LockCameraDuringDialogs)) {	
				ToggleCursor();
			}
		}
		
		// Teleport the player
		if (teleport_to_cursor.LocalPress() && GetGame().GetUIManager().IsCursorVisible()) {
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

		if (click_input.LocalPress() && m_DragBoxStartX == -1 && m_DragBoxStartY == -1) {
			if ((!widget_under_cursor || widget_under_cursor == EditorMapWidget) && GetGame().GetInput().HasGameFocus() && cursor_visible && !m_Editor.IsPlacing() && !m_Editor.IsDragging()) {
				m_DragBoxDelayStart = 0.12;
				GetMousePos(m_DragBoxStartX, m_DragBoxStartY);
				m_LassoHistory.Clear();
			}

			EditorMapWidget.SetFlags(WidgetFlags.IGNOREPOINTER);
		}
		
		if (click_input.LocalRelease()) {
			m_DragWidget = null;
			m_DragBoxDelayStart = 10;
			m_DragBoxStartX = -1;
			m_DragBoxStartY = -1;
			EditorMapWidget.ClearFlags(WidgetFlags.IGNOREPOINTER);
		}
		
		EditorCanvas.Clear();
		m_DragBoxDelayStart -= dt;
		if (click_input.LocalValue() && m_DragBoxDelayStart < 0 && GetGame().GetInput().HasGameFocus() && cursor_visible && !m_Editor.IsPlacing() && !m_Editor.IsDragging() && !m_Editor.GetBrush() && !m_DragWidget && m_DragBoxStartX != -1 && m_DragBoxStartY != -1) {	
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
							if (object_marker0 && object_marker0.GetEditorObject().IsSelected()) {
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
						if (!marker || !marker.GetLayoutRoot().IsVisible()) {
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
							if (object_marker && object_marker.GetEditorObject().IsSelected()) {
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
							if (object_marker2 && object_marker2.GetEditorObject().IsSelected()) {
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
				if (click_input.LocalPress()) {
					m_DragWidget = LeftbarWrapper;
				}
				
				if (click_input.LocalDoubleClick()) {
					LeftbarWrapper.GetSize(wr_s_w, wr_s_h);
					LeftbarWrapper.SetSize(DEFAULT_BAR_WIDTH_PX, wr_s_h);
				}
				
				break;
			}

			case RightbarDrag: {
				if (click_input.LocalPress()) {
					m_DragWidget = RightbarWrapper;
				}

				if (click_input.LocalDoubleClick()) {
					LeftbarWrapper.GetSize(wr_s_w, wr_s_h);
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
					float LeftWidth = Math.Clamp(mouse_x + wr_col_s_w, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX);
					LeftbarWrapper.SetScreenSize(LeftWidth, wr_s_h);
					m_Editor.GetSettings().LeftBarPlacement = LeftWidth;
					break;
				}

				case RightbarWrapper: {
					RightbarDrag0.SetColor(LinearColor.SLATE_BLUE);
					RightbarCollapsePanel.GetScreenSize(wr_col_s_w, wr_col_s_h);
					RightbarWrapper.GetScreenSize(wr_s_w, wr_s_h);
					float RightWidth = Math.Clamp(screen_x - mouse_x - wr_col_s_w, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX);
					RightbarWrapper.SetScreenSize(Math.Clamp(screen_x - mouse_x - wr_col_s_w, BAR_WIDTH_MINIMUM_PX, BAR_WIDTH_MAXIMUM_PX), wr_s_h);
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
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{		
		return super.OnClick(w, x, y, button);
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
				GetEditor().CancelPlacing();
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
				auto left_spacer_config = Ternary<ObservableCollection<ref EditorPlaceableListItem>>.If(m_TemplateController.CategoryConfig, m_TemplateController.LeftbarSpacerConfig, m_TemplateController.LeftbarSpacerStatic);
				for (int j = 0; j < left_spacer_config.Count(); j++) {
					int hide = !left_spacer_config[j].FilterType(left_search_bar_text);
					if (m_TemplateController.FavoritesToggle) {
						hide |= hide | (!left_spacer_config[j].GetTemplateController().Favorite << 1);
					}

					if (!m_TemplateController.ShowPrivate) {
						hide |= hide | (left_spacer_config[j].GetPlaceableItem().Scope < 2) << 2;
					}

					left_spacer_config[j].GetLayoutRoot().Show(!hide);
				}
				
				LeftbarScroll.VScrollToPos(0);
				
				Symbols left_search_bar_icon = Ternary<Symbols>.If(!left_search_bar_text.Length(), Symbols.MAGNIFYING_GLASS, Symbols.XMARK);
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
				Symbols right_search_bar_icon = Ternary<Symbols>.If(!right_search_bar_text.Length(), Symbols.MAGNIFYING_GLASS, Symbols.XMARK);
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
		super.Show(show);
		
		if (CurrentDialog) {
			CurrentDialog.GetLayoutRoot().Show(show);
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
		if (!CanCreateTooltip()) {
			return;
		}
		
		CurrentTooltip = current_tooltip;
	}

	TooltipView CreateDelayedTooltip(Widget w, string text, TooltipPosition position, string desc = string.Empty, Symbols icon = string.Empty, int delay = 300)
	{
		TooltipView view = TooltipView.CreateOnWidget(w, text, position, desc, icon);
		DelaySetCurrentTooltip(view, w, delay);
		return view;
	}

	void DelaySetCurrentTooltip(ScriptView current_tooltip, Widget w, int delay = 300)
	{
		CurrentTooltip = current_tooltip;
		CurrentTooltip.GetLayoutRoot().Show(false);

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(VerifyCurrentTooltip);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(VerifyCurrentTooltip, delay, false, w);
	}

	protected void VerifyCurrentTooltip(Widget w)
	{
		if (!CanCreateTooltip() || GetWidgetUnderCursor() != w) {
			return;
		}

		CurrentTooltip.GetLayoutRoot().Show(true);
	}

	protected bool CanCreateTooltip()
	{
		return !IsSelectionBoxActive();
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
