class EditorHud: ScriptView
{
	protected EditorHudController m_TemplateController;
		
	// View Properties
	Widget Left, Right, Inner, Tools, Menu, Floor;
	
	// Layout Elements
	Widget LeftDragZone, RightDragZone;
	
	CanvasWidget Whiteboard;
	ScrollWidget LeftScroll, RightScroll;
	
	Widget OutlinePanel, OnlineServerDetails;
	
	TextWidget Session, UserCountData, EntityCountData;
	
	Widget Notification;
	TextWidget NotificationText;
	
	protected Widget m_DraggedBar;
	protected int m_DragX = -1, m_DragY = -1;
		
	protected ref ScriptView m_CurrentMenu;
	
	protected ref map<string, ref EditorCategory> m_PlacedCategories = new map<string, ref EditorCategory>();
	protected ref map<string, ref EditorCategory> m_PlaceableCategories = new map<string, ref EditorCategory>();
	
	protected EditorCategory m_CurrentPlacingContext;
	
	protected ref array<ref EditorPlaceableObject> m_Placeables = {};
	protected ref array<ref EditorObjectDataCategory> m_Data = {};

	void EditorHud()
	{		
		m_TemplateController = EditorHudController.Cast(m_Controller);
		
		float w, h;
		int x, y;
				
		// Script sizing for menu bars and whatnot
		GetScreenSize(x, y);
		Left.GetSize(w, h);
		Left.SetSize(w, y - 110);
		Right.GetSize(w, h);
		Right.SetSize(w, y - 110);	
			
		m_CurrentPlacingContext = InsertPlacedCategory(new EditorCategory(new EditorObjectDataCategory("Placed Objects")));
		InsertPlacedCategory(new EditorCategory(new EditorObjectDataCategory("Hidden Objects")));
		InsertPlacedCategory(new EditorCategory(new EditorObjectDataCategory("Brushed Objects")));
		
		map<EditorObjectDataCategory, ref array<EditorPlaceableObjectData>> placeable_items = GetDayZGame().GetPlaceableItemsByCategory();
		foreach (EditorObjectDataCategory category, array<EditorPlaceableObjectData> items: placeable_items) {
			m_PlaceableCategories[category.GetName()] = new EditorCategory(category);
			m_TemplateController.LeftListItems.Insert(m_PlaceableCategories[category.GetName()].GetTreeItem());
			
			foreach (EditorPlaceableObjectData data: items) {
				EditorPlaceableObject placeable_object = new EditorPlaceableObject(data);
				m_Placeables.Insert(placeable_object);
				m_PlaceableCategories[category.GetName()].AddChild(placeable_object);
			}
		}	
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		int x, y;
		GetScreenSize(x, y);
		
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);
		
		if (GetGame().GetInput().LocalPress("UAFire") && !GetWidgetUnderCursor()) {
			EditorSelectableBase.ClearSelections();
		}
		
		if (GetEditor().GetCurrentOnlineSession()) {
			m_TemplateController.RotatingJoinCode = GetEditor().GetCurrentOnlineSession().GetJoinCode();
			m_TemplateController.NotifyPropertyChanged("RotatingJoinCode");
		}
				
		//EntityCountData.SetText(GetEditor().m_PlacedObjects.Count().ToString());
		//Print(GetEditor().GetCurrentOnlineSession());
		//OnlineServerDetails.Show(GetEditor().GetCurrentOnlineSession() != null);
						
		Whiteboard.Clear();
		if ((GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) == MB_PRESSED_MASK && m_DragX != -1 && m_DragY != -1) {												
			// Handles the fill operation
			int x_avg = (m_DragX + mouse_x) / 2;
			Whiteboard.DrawLine(x_avg, m_DragY, x_avg, mouse_y, mouse_x - m_DragX, 0x644B77BE); 
						
			foreach (EditorSelectableBase selectable_item: EditorSelectableBase.All) {
				EditorTreeItem view = selectable_item.GetTreeItem();
				if (view) {
					float tree_x, tree_y;
					view.GetLayoutRoot().GetScreenPos(tree_x, tree_y);
										
					if ((tree_x < Math.Max(m_DragX, mouse_x) && tree_x > Math.Min(m_DragX, mouse_x)) && (tree_y < Math.Max(m_DragY, mouse_y) && tree_y > Math.Min(m_DragY, mouse_y))) {
						selectable_item.SetSelected(true);
					}
				}				
			}
		}

		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
			m_DragX = -1; m_DragY = -1;
		}
							
		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) && m_DraggedBar) {
			m_DraggedBar.SetColor(COLOR_WHITE);
			m_DraggedBar = null;
		}		
		
		if (m_DraggedBar) {
			switch (m_DraggedBar.GetParent()) {
				case Right: {
					Right.SetSize(x - Math.Clamp(mouse_x, x - 720, x - 60), y - 74);
					m_DraggedBar.SetColor(COLOR_BLUE);
					break;
				}
				
				case Left: {
					Left.SetSize(Math.Clamp(mouse_x, 60, 720), y - 74);
					m_DraggedBar.SetColor(COLOR_BLUE);
					break;
				}
			}			
		}
		
		float w, h;
		float inner_width = x;
		float inner_height = y;
		Left.GetSize(w, h);
		float inner_x = w;
		
		inner_width -= w;
		Right.GetSize(w, h);
		inner_width -= w;
		
		Tools.GetSize(w, h);
		float inner_y = h;
		inner_height -= h;
		
		float floor_width, floor_height;
		Floor.GetSize(floor_width, floor_height);
		
		// Set size of inner
		Inner.SetSize(inner_width, inner_height);
		Floor.SetSize(inner_width, floor_height);
		Floor.SetPos(inner_x, 0);
		Inner.SetPos(inner_x, inner_y);	
		
		if (GetGame().GetInput().LocalPress("EditorToggleCursor")) {
			ShowCursor(!IsCursorVisible());
		}
	}
	
	void OnWebExecute(ButtonCommandArgs args)
	{
		OutlinePanel.Show(!OutlinePanel.IsVisible());
	}
		
	void OnJoinExecute(ButtonCommandArgs args)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(m_TemplateController.JoinCode.ToString());
		rpc.Send(null, EditorOnlineSessionManager.RPC_REQUEST_JOIN_SESSION, false);
	}
	
	void OnHostExecute(ButtonCommandArgs args)
	{
		ScriptRPC().Send(null, EditorOnlineSessionManager.RPC_REQUEST_CREATE_SESSION, true);
	}
	
	EditorCategory InsertPlacedCategory(notnull EditorCategory editor_category)
	{
		m_PlacedCategories[editor_category.GetName()] = editor_category;
		m_TemplateController.RightListItems.Insert(editor_category.GetTreeItem());
		
		return editor_category;
	}
	
	EditorCategory GetPlacedCategory(string category)
	{
		return m_PlacedCategories[category];
	}
	
	EditorCategory GetCurrentPlacingCategory()
	{
		return m_CurrentPlacingContext;
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
			
	void OnCopySessionExecute(ButtonCommandArgs args)
	{
		string code = GetEditor().GetCurrentOnlineSession().GetJoinCode();
		GetGame().CopyToClipboard(code);
		ShowNotification(string.Format("Code %1 copied to clipboard", code));
	}
	
	void OnLeaveSessionExecute(ButtonCommandArgs args)
	{
		ScriptRPC().Send(null, EditorOnlineSessionManager.RPC_REQUEST_LEAVE_SESSION, true);
	}
	
	void OnCreateNewFolder(ButtonCommandArgs args)
	{
		//m_TemplateController.RightListItems.Insert(new EditorTreeItem("New Folder", null));
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		return super.OnFocus(w, x, y);
	}

	override bool OnFocusLost(Widget w, int x, int y)
	{
		
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
		ViewBinding view_binding = m_TemplateController.GetViewBinding(w);
		if (view_binding && !m_CurrentMenu) {
			/*EditorCommand editor_command;
			if (Class.CastTo(editor_command, view_binding.GetRelayCommand())) {
				
				float pos_x, pos_y, size_x, size_y;
				w.GetScreenPos(pos_x, pos_y);
				w.GetScreenSize(size_x, size_y);
				
				//m_CurrentTooltip = EditorTooltip.CreateOnButton(editor_command, w, TooltipPositions.BOTTOM_LEFT);
				if (!editor_command.CanExecute()) {
					m_CurrentTooltip.GetLayoutRoot().SetAlpha(100);
				}
			}*/
		}
					
		switch (w) {
			case LeftDragZone:
			case RightDragZone: {
				//WidgetAnimator.AnimateColorHSV(w, "240 140 60", "239 131 175", 30);
				//LeftDragZone.SetColor(COLOR_WHITE);
				//WidgetAnimator.AnimateColor(w, COLOR_BLUE_LIGHT, 60);
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
				//WidgetAnimator.AnimateColor(w, COLOR_BLUE, 50);
				//LeftDragZone.SetColor(COLOR_SALMON_A);
				//WidgetAnimator.AnimateColor(w, COLOR_WHITE, 60);
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
			
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		switch (w) {
			/*
			case SearchBar: {
				for (int i = 0; i < m_TemplateController.LeftListItems.Count(); i++) {					
					string text;
					SearchBar.GetText(text);
					m_TemplateController.LeftListItems[i].ApplyFilter(text);
				}
				
				break;
			}*/
		}
		
		return super.OnChange(w, x, y, finished);
	}
	
	override void Show(bool show) 
	{
		m_LayoutRoot.Show(show);
		
		if (CurrentDialog) {
			CurrentDialog.GetLayoutRoot().Show(show);
		}
		
		if (IsCursorVisible() && !show) {
			ShowCursor(false);
		}
		
		Man controlled_player = GetEditor().GetCurrentControlPlayer();
		Hud hud = GetGame().GetMission().GetHud();
		hud.ShowHudUI(g_Game.GetProfileOption(EDayZProfilesOptions.HUD) && !show && controlled_player != null);
		hud.ShowQuickbarUI(g_Game.GetProfileOption(EDayZProfilesOptions.QUICKBAR) && !show && controlled_player != null);
	}
	
	override bool IsVisible() 
	{
		return m_LayoutRoot.IsVisible();
	}
		
	void ShowCursor(bool state) 
	{
		GetGame().GetUIManager().ShowCursor(state);
		
		// If player is active
		Man controlled_player = GetEditor().GetCurrentControlPlayer();
		if (controlled_player) {
			controlled_player.DisableSimulation(IsCursorVisible());
		}
		
		if (!state) {
			delete CurrentTooltip;
			SetFocus(null);
		}
	}
	
	bool IsCursorVisible()
	{
		return GetGame().GetUIManager().IsCursorVisible();
	}
				
	void ShowNotification(string text, int color = -9137292, float duration = 4.0)
	{
		Notification.SetColor(color);
		NotificationText.SetText(text);
				
		WidgetAnimator.Animate(Notification, WidgetAnimatorProperty.POSITION_Y, 0, 25, 30);
		WidgetAnimator.Animate(Notification, WidgetAnimatorProperty.COLOR_A, 0.0, 1.0, 30);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(HideNotification);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(HideNotification, 1000 * duration);
	}
	
	void HideNotification()
	{
		WidgetAnimator.Animate(Notification, WidgetAnimatorProperty.COLOR_A, 0.0, 250);
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
		string text = GetFriendlyObjectName(target, component_index);
		//ObjectHoverText.SetText(text);
		//ObjectHoverText.SetColor(Ternary<int>.If(text.Contains(".p3d"), COLOR_YELLOW, COLOR_WHITE));
		return true;
	}
	
	bool OnMouseExitObject(Object target, int x, int y, int component_index)
	{	
		//ObjectHoverText.SetText(string.Empty);
		return true;
	}	
	
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
		
		array<EditorPlaceableObjectData> placeable_items = GetDayZGame().GetReplaceableObjects(split_string[1].Trim());
		// not ideal since we dont want to feed them the p3d, but doable
		if (!placeable_items || placeable_items.Count() == 0) {
			return string.Format("%1 [%2, %3: %4]", split_string[1], split_string[0], component_type, component_index);
		}
				
		return string.Format("%1 [%2, %3: %4]", placeable_items[0].GetName(), split_string[0], component_type, component_index);
	}
		
	// ToolTip Control
	static ref ScriptView CurrentTooltip;

	// Dialog Control`
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
