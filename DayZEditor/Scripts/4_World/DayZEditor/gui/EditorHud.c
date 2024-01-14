class EditorHud: ScriptView
{
	protected bool m_IsBoxSelectActive;
	protected EditorHudController m_TemplateController;
	
	// View Properties
	protected ButtonWidget MenuBarFile;
	protected ButtonWidget MenuBarEdit;
	protected ButtonWidget MenuBarView;
	protected ButtonWidget MenuBarEditor;
	
	// View Properties
	Widget Left;
	
	// Layout Elements
	Widget NotificationFrame;
	Widget MapContainer;
	Widget LoggerFrame;
	Widget LeftDragZone;
	Widget PlaceablesList;
	
	CanvasWidget EditorCanvas;
	ScrollWidget PlaceablesScroll;
	
	protected MultilineEditBoxWidget SearchBar;
	
	ref EditorCameraMapMarker CameraMapMarker;
	
	// todo protect this and move all Map logic in here?
	MapWidget EditorMapWidget;
	
	EditBoxWidget LeftbarSearchBar;
	
	protected ref EditorMenu m_CurrentMenu;
	protected ref EditorTooltip m_CurrentTooltip;
	
	protected Widget m_DraggedBar;
	
	void EditorHud()
	{	
		ShowScreenLogs(GetEditor().Settings.ShowScreenLogs);
	}
		
	override void Update(float dt)
	{
		super.Update(dt);
		
		m_TemplateController = EditorHudController.Cast(m_Controller);
		
		float w, h;
		int x, y;
		GetScreenSize(x, y);
		Left.GetSize(w, h);
		Left.SetSize(w, y - 74);
		PlaceablesScroll.SetSize(1.0, y - 74 - 28);

		if (GetEditor().GetCamera()) {
			vector cam_pos = GetEditor().GetCamera().GetPosition();
			
			m_TemplateController.cam_x = cam_pos[0];
			m_TemplateController.cam_y = cam_pos[1];
			m_TemplateController.cam_z = cam_pos[2];
			m_TemplateController.NotifyPropertiesChanged({ "cam_x", "cam_y", "cam_z"} );
		}
		
		array<EditorCommand> commands = GetEditor().CommandManager.GetCommands();
		foreach (EditorCommand command: commands) {
			auto view_binding = command.GetViewBinding();
			if (!view_binding) {
				continue;
			}
			
			Widget root = view_binding.GetLayoutRoot();
			bool can_execute = command.CanExecute();
			if (can_execute) {
				root.SetAlpha(1);
			} else {
				root.SetAlpha(0.25);
			}
			
			root.Enable(can_execute);			
		}
				
		// kinda cursed but double inputs. maybe have a handler if you want more ui shit (loooot editor)
		if (GetEditor().IsInventoryEditorActive() || (GetFocus() && GetFocus().IsInherited(EditBoxWidget))) {
			return;
		}
				
		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
			m_DraggedBar = null;
		}
		
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);
	
		if (m_DraggedBar) {			
			m_DraggedBar.GetParent().GetSize(w, h);
			m_DraggedBar.GetParent().SetSize(Math.Clamp(mouse_x, 40, 720), y - 74);
		}
		
		// minimum placeables list sizing looks better
		PlaceablesList.GetSize(w, h);
		PlaceablesList.SetSize(w, Math.Max(h, y));
		
		Input input = GetGame().GetInput();
		if (input.LocalPress("EditorPlaceObjectCommand") && !KeyState(KeyCode.KC_LSHIFT) && !GetWidgetUnderCursor()) {
			GetEditor().ClearSelection();
			
			DelayedDragBoxCheck();
		}
		
		if (input.LocalPress("EditorToggleCursor")) {
			if (!EditorMapWidget.IsVisible() && (!CurrentDialog || !GetEditor().Settings.LockCameraDuringDialogs)) {
				ShowCursor(!IsCursorVisible());
			}
		}
		
		if (input.LocalPress("EditorToggleMap")) {
			EditorMapWidget.Show(!EditorMapWidget.IsVisible());
			ShowCursor(true);
		}
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (w) {
			case LeftDragZone: {
				m_DraggedBar = LeftDragZone;
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		ViewBinding view_binding = m_TemplateController.GetViewBinding(w);
		if (view_binding && !m_CurrentMenu) {
			EditorCommand editor_command;
			if (Class.CastTo(editor_command, view_binding.GetRelayCommand())) {
				
				float pos_x, pos_y, size_x, size_y;
				w.GetScreenPos(pos_x, pos_y);
				w.GetScreenSize(size_x, size_y);
								
				m_CurrentTooltip = EditorTooltip.CreateOnButton(editor_command, w, TooltipPositions.BOTTOM_LEFT);
				if (!editor_command.CanExecute()) {
					m_CurrentTooltip.GetLayoutRoot().SetAlpha(100);
				}
			}
		}
				
		
		switch (w) {
			case LeftDragZone: {
				//WidgetAnimator.AnimateColorHSV(LeftDragZone, "240 140 60", "239 131 175", 30);
				//LeftDragZone.SetColor(COLOR_WHITE);
				WidgetAnimator.AnimateColor(LeftDragZone, COLOR_WHITE, 50);
				break;
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case LeftDragZone: {
				WidgetAnimator.AnimateColor(LeftDragZone, COLOR_SALMON_A, 50);
				//LeftDragZone.SetColor(COLOR_SALMON_A);
				break;
			}
		}
		
		delete m_CurrentTooltip;
		return super.OnMouseLeave(w, enterW, x, y);
	}
		
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w) {
			case MenuBarFile:
			case MenuBarEdit:
			case MenuBarView:
			case MenuBarEditor: {
				m_CurrentMenu = CreateToolbarMenu(w);
				return true;
			}		
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		switch (w) {
			case SearchBar: {
				for (int i = 0; i < m_TemplateController.Placeables.Count(); i++) {
					EditorFolderTreeItem tree_item = EditorFolderTreeItem.Cast(m_TemplateController.Placeables[i]);
					if (!tree_item) {
						continue;
					}
					
					string text;
					SearchBar.GetText(text);
					tree_item.ApplyFilter(text);
				}
				
				break;
			}
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
		
		PlayerBase controlled_player = GetEditor().GetCurrentControlPlayer();
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
		PlayerBase controlled_player = GetEditor().GetCurrentControlPlayer();
		if (controlled_player) {
			controlled_player.DisableSimulation(IsCursorVisible());
		}
		
		if (!state) {
			delete CurrentTooltip;
			delete CurrentMenu;
			SetFocus(null);
		}
	}
	
	bool IsCursorVisible()
	{
		return GetGame().GetUIManager().IsCursorVisible();
	}
		
	void ShowScreenLogs(bool state)
	{
		LoggerFrame.Show(state);
	}
		
	void CreateNotification(string text, int color = -4301218, float duration = 4)
	{
		EditorLog.Trace("EditorHud::CreateNotification");
		
		GetTemplateController().ShowNotification(text, color, duration);
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
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(_DelayedDragBoxCheck, 0, true, x, y);
	}

	private void _DelayedDragBoxCheck(int start_x, int start_y)
	{
		int drag_box_color = GetEditor().Settings.SelectionColor;
		
		int a, r, g, b;
		InverseARGB(drag_box_color, a, r, g, b);
		int drag_box_color_fill = ARGB(50, r, g, b);			
		
		int current_x, current_y;
		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) || !GetGame().GetInput().HasGameFocus()) {	
			EditorCanvas.Clear();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(_DelayedDragBoxCheck);
			return;
		}
		
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
	
	EditorHudController GetTemplateController()
	{
		return EditorHudController.Cast(m_Controller);
	}
	
	// Relay Commands
	void MenuBarExecute(ButtonCommandArgs args) 
	{		
		EditorLog.Trace("EditorHudToolbarController::MenuBarExecute");
		if (!EditorHud.CurrentMenu) { //  GetMenu().Type() != GetBoundMenu(args.GetButtonWidget()) removed cause GetBoundMenu is gone
			EditorHud.CurrentMenu = CreateToolbarMenu(args.Source);
		} else {
			delete EditorHud.CurrentMenu;
		}
	}	
	
	protected EditorMenu CreateToolbarMenu(Widget toolbar_button)
	{
		EditorLog.Trace("EditorHudToolbarController::CreateToolbarMenu");	
				
		EditorMenu toolbar_menu;
		switch (toolbar_button) {
			
			case MenuBarFile: {
				toolbar_menu = new EditorFileMenu();
				break;
			}
			
			case MenuBarEdit: {
				toolbar_menu = new EditorEditMenu();
				break;
			}
			
			case MenuBarView: {
				toolbar_menu = new EditorViewMenu();
				break;
			}
			
			case MenuBarEditor: {
				toolbar_menu = new EditorEditorMenu(); // lol
				break;
			}
		}
		
		// Sets position to bottom of button
		float x, y, w, h;
		toolbar_button.GetScreenPos(x, y);
		toolbar_button.GetScreenSize(w, h);
		toolbar_menu.GetLayoutRoot().SetPos(x, y + h);
		
		return toolbar_menu;
	}
	
	bool OnMouseEnterObject(Object target, int x, int y, int component_index)
	{
		m_TemplateController.ObjectReadoutName = GetFriendlyObjectName(target, component_index);
		m_TemplateController.NotifyPropertyChanged("ObjectReadoutName");
		
		if (m_TemplateController.ObjectReadoutName.Contains(".p3d")) { // yeah its hacky but its cool!
			m_TemplateController.ObjectHoverSelectObjectReadout.SetColor(COLOR_YELLOW);
		} else {
			m_TemplateController.ObjectHoverSelectObjectReadout.SetColor(COLOR_WHITE);
		}
		
		return true;
	}
	
	bool OnMouseExitObject(Object target, int x, int y, int component_index)
	{
		m_TemplateController.ObjectReadoutName = "";
		m_TemplateController.NotifyPropertyChanged("ObjectReadoutName");
	
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
		
		array<EditorPlaceableItem> placeable_items = GetEditor().GetObjectManager().GetReplaceableObjects(split_string[1].Trim());
		// not ideal since we dont want to feed them the p3d, but doable
		if (!placeable_items || placeable_items.Count() == 0) {
			return string.Format("%1 [%2, %3: %4]", split_string[1], split_string[0], component_type, component_index);
		}
				
		return string.Format("%1 [%2, %3: %4]", placeable_items[0].GetName(), split_string[0], component_type, component_index);
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
