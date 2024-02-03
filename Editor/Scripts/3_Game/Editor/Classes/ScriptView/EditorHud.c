class EditorHud: ScriptView
{		
	static const string SEARCH_BAR_DEFAULT = "Search...";
	static const ref array<string> LEFT_NODES = { EditorNode.PLACEABLES, EditorNode.BRUSHES, EditorNode.DZ, EditorNode.PLACING };
	static const ref array<string> RIGHT_NODES = { EditorNode.HISTORY, EditorNode.LAYERS };
	
	protected EditorNode m_Editor;
	protected EditorHudController m_TemplateController;
	
	Widget Cursor, CursorTooltip;
	TextWidget CursorTooltipName;
	ImageWidget Foreground, Background;
	
	Widget CursorEntity;
	ItemPreviewWidget CursorEntityPreview;
	protected Object m_TooltipObject;
		
	// View Properties
	Widget Left, Right, Inner, Tools, Menu, Top;
	
	MapWidget Map;
	
	// Layout Elements
	Widget LeftDragZone, RightDragZone;
	
	CanvasWidget Whiteboard;
	ScrollWidget LeftScroll, RightScroll;
	
	Widget OutlinePanel, OnlineServerDetails;
	
	TextWidget Session, UserCountData, EntityCountData;
	
	Widget Notification;
	TextWidget NotificationText;
	
	EditBoxWidget SearchBar;
	
	ItemPreviewWidget Item;
	PlayerPreviewWidget Player;
	
	SelectionMode CurrentSelectionMode = SelectionMode.BOX;
	
	protected ref array<vector> m_LassoHistory = {};
	
	protected Widget m_DraggedBar;
	protected int m_DragX = -1, m_DragY = -1;
	protected vector m_MapDrag;
		
	void EditorHud(notnull EditorNode editor)
	{		
		m_Editor = editor;
		m_TemplateController = EditorHudController.Cast(m_Controller);
				
		GetGame().GetMission().GetHud().ShowHudUI(false);
		GetGame().GetMission().GetHud().ShowQuickbarUI(false);
			
		foreach (string left: LEFT_NODES) {
			m_TemplateController.LeftListItems.Insert(editor[left].CreateView());
		}
		
		foreach (string right: RIGHT_NODES) {
			m_TemplateController.RightListItems.Insert(editor[right].CreateView());
		}
		
		m_TemplateController.RightListItems.Insert(TreeNode.ROOT[RootNode.MISSION].CreateView());
	}
	
	void ~EditorHud()
	{
		SetCursorWidget(null);
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		Widget root_widget = EnScriptVar<Widget>.Get(GetGame().GetMission(), "m_HudRootWidget");
		root_widget.Show(!m_Editor.IsActive());
		
		int x, y;
		GetScreenSize(x, y);
		
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);
		
		Input input = GetGame().GetInput();	
		
		if (input.LocalPress_ID(UAFire) && TreeNode.StateMachine[TreeNodeState.HOVER].Count() == 0) {
			m_DragX = mouse_x;
			m_DragY = mouse_y;
		}
		
		Whiteboard.Clear();
		if (input.LocalHold_ID(UAFire) && !m_DraggedBar && m_DragX != -1 && m_DragY != -1) {	
			switch (CurrentSelectionMode) {
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
						Whiteboard.DrawLine(m_LassoHistory[j][0], m_LassoHistory[j][1], m_LassoHistory[j + 1][0], m_LassoHistory[j + 1][1], 4, EditorColor.SELECT);
					}
					
					if (m_LassoHistory.Count() > 2) {
						Whiteboard.DrawLine(m_LassoHistory[0][0], m_LassoHistory[0][1], m_LassoHistory[m_LassoHistory.Count() - 1][0], m_LassoHistory[m_LassoHistory.Count() - 1][1], 4, EditorColor.SELECT & 0xA0FFFFFF);
					}
					
					break;
				}
				
				case SelectionMode.BOX: {
			
					// Rectangle
					int x_avg = (m_DragX + mouse_x) / 2;
					int y_avg = (m_DragY + mouse_y) / 2;
					Whiteboard.DrawLine(x_avg, m_DragY, x_avg, mouse_y, mouse_x - m_DragX, 0x644B77BE);		
					
					
					/*foreach (TreeNode node: TreeNode.All) {
						if (node && node.GetNodeView()) {
							float x_n, y_n;
							node.GetNodeView().GetLayoutRoot().GetScreenPos(x_n, y_n);
							if ((x_n < Math.Max(m_DragX, mouse_x) && x_n > Math.Min(m_DragX, mouse_x)) && (y_n < Math.Max(m_DragY, mouse_y) && y_n > Math.Min(m_DragY, mouse_y))) {
								node.SetSelected(true);
							}
						}
					}*/
					
					break;
				}
				
				case SelectionMode.ELLIPSE: {
					
					int width = mouse_x - m_DragX;
					int height = mouse_y - m_DragY;
					
					int x_middle = m_DragX - (width / 2);
					int y_middle = m_DragY - (height / 2);
					
					
			        // Center of the oval
			        float cx = width / 2;
			        float cy = height / 2;
			
			        // Radius for x and y axes
			        float rx = width / 2;
			        float ry = height / 2;
			
			        // Y-coordinate for the horizontal lines	
			        // Calculate the horizontal lines to approximate the oval
			        for (int i = 0; i < 100; i++)
			        {
			            // Calculate points on the oval using parametric equations
			            float x1 = cx - rx;
			            float x2 = cx + rx;
			
			            // Draw a horizontal line at the current y-coordinate
			            Whiteboard.DrawLine(x1 + x_middle, cy + y_middle, x2 + x_middle, cy + 1 + y_middle, 1, 0x644B77BE);
			
			            // Update the y-coordinate for the next horizontal line
			            cy += 2 * ry / 100.0;
			        }
					
					break;
				}
			}
		}
		
		if (input.LocalRelease_ID(UAFire)) {

			switch (CurrentSelectionMode) {
				case SelectionMode.LASSO: {
					foreach (Object object, ObjectNode object_node: ObjectNode.All) {
						ObjectView view = object_node.GetObjectViewWorld();
						
						float x_node_screen, y_node_screen;
						view.GetLayoutRoot().GetScreenPos(x_node_screen, y_node_screen);
						if (m_LassoHistory.Count() > 0) {							
							if (IsPointInPolygon(x_node_screen, y_node_screen, m_LassoHistory)) {
								object_node.AddState(TreeNodeState.ACTIVE);
							}
						}
					}
					
					break;
				}
			}
			
			m_DragX = -1;
			m_DragY = -1;
			m_LassoHistory.Clear();
		}
									
		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) && m_DraggedBar) {
			m_DraggedBar.GetChildren().SetColor(COLOR_WHITE);
			m_DraggedBar = null;
			ClearCursor();
		}		
				
		float top_width, top_height;
		Top.GetScreenSize(top_width, top_height);
		
		float tools_width, tools_height;
		Tools.GetScreenSize(tools_width, tools_height);
		
		float left_width, left_height;
		Left.GetScreenSize(left_width, left_height);
		Left.SetScreenSize(left_width, x - top_height - tools_height);
		
		float right_width, right_height;
		Right.GetScreenSize(right_width, right_height);
		Right.SetScreenSize(right_width, x - top_height - tools_height);
		
		// Set size of inner
		Inner.SetSize(x - right_width - left_width, y - tools_height - top_height);
		Inner.SetPos(left_width, tools_height + top_height);	
		
		if (m_DraggedBar) {
			int distance_from_wall = mouse_x;
			if (distance_from_wall > x / 2) {
				distance_from_wall = x - distance_from_wall;
			}
			
			m_DraggedBar.GetParent().SetSize(distance_from_wall, y - (tools_height + top_height));
			m_DraggedBar.GetChildren().SetColor(EditorColor.SELECT);
		}
		
		if (input.LocalValue_ID(UAUIBack)) {
			//EnterChildMenu(MENU_INGAME);
		}
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

	void SetCursor(Symbols cursor, string name = string.Empty, string type = string.Empty)
	{	
		CursorTooltip.Show(name != string.Empty || type != string.Empty);
		
		m_TooltipObject = GetGame().CreateObjectEx(type, vector.Zero, ECE_LOCAL);

		CursorEntity.Show(false);
		if (m_TooltipObject) {
			EntityAI entity = EntityAI.Cast(m_TooltipObject);
			if (entity) {
				CursorEntity.Show(true);
				CursorEntityPreview.SetItem(entity);
				CursorEntityPreview.SetView(entity.GetViewIndex());
			}
		}
		
		CursorTooltipName.SetText(name);
		
		if (cursor != string.Empty) {
			Foreground.LoadImageFile(0, cursor.Thin());
			Foreground.SetImage(0);
			Background.LoadImageFile(0, cursor.Solid());
			Background.SetImage(0);
		}
		
		Foreground.Show(name == string.Empty);
		Background.Show(name == string.Empty);
		
		
		Cursor.Show(true);
		SetCursorWidget(Cursor);
	}
	
	void ClearCursor()
	{
		Background.Show(false);
		Foreground.Show(false);
		CursorTooltip.Show(false);
		CursorEntity.Show(false);
		Cursor.Show(false);
		GetGame().ObjectDelete(m_TooltipObject);
	}
		
	void OnDiscordButtonExecute(ButtonCommandArgs args)
	{
		
	}
	
	void OnAfterlifeExecute(ButtonCommandArgs args)
	{
		array<Object> objects = {};
		GetGame().GetObjectsAtPosition(GetGame().GetCurrentCameraPosition(), 300, objects, null);
		
		foreach (Object object: objects) {
			bool hidden = CF.ObjectManager.IsMapObjectHidden(object);
			
			if (hidden) {
				CF.ObjectManager.UnhideMapObject(object, false);
			} else {
				CF.ObjectManager.HideMapObject(object, false);
			}
		}	
	}
	
	void OnCreateNewFolder(ButtonCommandArgs args)
	{
		//m_TemplateController.RightListItems.Insert(new TreeView("New Folder", null));
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		switch (w) {
			case SearchBar: {
				if (SearchBar.GetText() == SEARCH_BAR_DEFAULT) {
					SearchBar.SetText(string.Empty);
				}
				
				return true;
			}
		}
		
		return super.OnFocus(w, x, y);
	}

	override bool OnFocusLost(Widget w, int x, int y)
	{
		switch (w) {
			case SearchBar: {
				if (SearchBar.GetText() == string.Empty) {
					SearchBar.SetText(SEARCH_BAR_DEFAULT);
				}
				
				return true;
			}
		}
			
		return super.OnFocusLost(w, x, y);
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (w) {			
			case LeftDragZone:
			case RightDragZone: {
				m_DraggedBar = w;
				break;
			}
		}
		
		if (!w) {
			m_DragX = x; m_DragY = y;
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{					
		switch (w) {
			case LeftDragZone:
			case RightDragZone: {
				SetCursor(Symbols.LEFT_RIGHT);
				break;
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case LeftDragZone:
			case RightDragZone: {
				if (!m_DraggedBar) {
					ClearCursor();
				}
				
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
			
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		switch (w) {
			case SearchBar: {
				//if (finished) {
					for (int i = 0; i < m_TemplateController.LeftListItems.Count(); i++) {
						m_TemplateController.LeftListItems[i].ApplyFilter(SearchBar.GetText());
					}
					
					for (int j = 0; j < m_TemplateController.RightListItems.Count(); j++) {
						m_TemplateController.RightListItems[j].ApplyFilter(SearchBar.GetText());
					}
				//}
				
				break;
			}
		}
		
		return super.OnChange(w, x, y, finished);
	}
							
	void ShowNotification(string text, int color = -9137292, float duration = 4.0)
	{
		Notification.SetColor(color);
		NotificationText.SetText(text);
		
		WidgetAnimator.Animate(Notification, WidgetAnimatorProperty.POSITION_Y, 0, 90);
		WidgetAnimator.Animate(Notification, WidgetAnimatorProperty.COLOR_A, 0.0, 1.0, 90);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(HideNotification);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(HideNotification, 1000 * duration);
		
		m_Editor.PlaySound("Lock_SoundSet");
	}
	
	void HideNotification()
	{
		WidgetAnimator.Animate(Notification, WidgetAnimatorProperty.COLOR_A, -20, 250);
	}
	
	void ShowRuleOfThirds(bool state)
	{
		if (!state) {
			Whiteboard.Clear();
			return;
		}
		
		int x, y;

		GetScreenSize(x, y);				
		Whiteboard.DrawLine(x / 3, 0, x / 3, y, 1, COLOR_BLACK);
		Whiteboard.DrawLine((x / 3) * 2, 0, (x / 3) * 2, y, 1, COLOR_BLACK);
		
		Whiteboard.DrawLine(0, y / 3, x, y / 3, 1, COLOR_BLACK);
		Whiteboard.DrawLine(0, (y / 3) * 2, x, (y / 3) * 2, 1, COLOR_BLACK);
	}
		
	override string GetLayoutFile() 
	{
		return "Editor/gui/layouts/hud/EditorHud.layout";
	}
	
	override typename GetControllerType()
	{
		return EditorHudController;
	}
	
	EditorHudController GetTemplateController()
	{
		return EditorHudController.Cast(m_Controller);
	}
			
	bool OnMouseEnterObject(Object target, int x, int y, int component_index)
	{
		//string text = GetFriendlyObjectName(target, component_index);
		//ObjectHoverText.SetText(text);
		//ObjectHoverText.SetColor(Ternary<int>.If(text.Contains(".p3d"), COLOR_YELLOW, COLOR_WHITE));
		return true;
	}
	
	bool OnMouseExitObject(Object target, int x, int y, int component_index)
	{	
		//ObjectHoverText.SetText(string.Empty);
		return true;
	}	
	
	/*
	static string GetFriendlyObjectName(notnull Object object, int component_index)
	{		
		while (object.GetParent()) {
			object = Object.Cast(object.GetParent());
		}
		
		string component_type = "component";
		Building building = Building.Cast(object);
		if (building) {
			if (building.GetDoorIndex(component_index) != -1) {
				component_index = building.GetDoorIndex(component_index);
				component_type = "door";
			}
		}
		
		if (object.GetType() != string.Empty && !object.IsTree() && !object.IsBush() && !object.IsRock()) {			
			return string.Format("%1 [%2, %3: %4]", object.GetType(), object.GetID(), component_type, component_index);
		}
		
		// 1346854: tank_small_white.p3d
		string debug_name = object.GetDebugNameNative();
		if (debug_name == string.Empty) {
			// lost cause, unlikely
			return string.Empty;
		}
		
		array<string> split_string = {};
		debug_name.Split(":", split_string);
		
		// also unlikely
		if (split_string.Count() == 1) {
			return string.Empty;
		}
		
		array<PlaceableNodeObjectData> placeable_items = GetDayZGame().GetReplaceableObjects(split_string[1].Trim());
		// not ideal since we dont want to feed them the p3d, but doable
		if (!placeable_items || placeable_items.Count() == 0) {
			return string.Format("%1 [%2, %3: %4]", split_string[1], split_string[0], component_type, component_index);
		}
				
		return string.Format("%1 [%2, %3: %4]", placeable_items[0].GetName(), split_string[0], component_type, component_index);
	}*/
}
