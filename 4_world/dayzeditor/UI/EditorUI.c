



class EditorMap: EditorWidgetEventHandler
{

	void EditorMap()
	{
		Print("EditorMap");
				
	}
	
	void ~EditorMap()
	{
		Print("~EditorMap");
	}
	
	void OnObjectCreated(Class context, EditorObject obj)
	{
		MapWidget map_widget = MapWidget.Cast(m_Root);
		map_widget.AddChild(obj.GetMapMarker());
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("EditorMap::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		EditorEvents.OnObjectCreated.Insert(OnObjectCreated);		
	}
	
	private int start_x, start_y;
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Input input = GetGame().GetInput();	
		MapWidget map_widget = GetMapWidget();
		
		if (button == 0) {
			if (Editor.IsPlacing()) {
				EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
				vector mat[4];
				e.GetTransform(mat);
				EditorObject editor_object = Editor.CreateObject(e.GetType(), mat);
				editor_object.Select();
				if (!input.LocalValue("UATurbo")) delete Editor.ObjectInHand;
				return true;
			} else {
				EditorUI.EditorCanvas.Clear();
				EditorUI ui = EditorUI.GetInstance();
				GetCursorPos(ui.start_x, ui.start_y);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ui.DelayedDragBoxCheck, 40);
				return true;
			}
			
		} else if (button == 2) {
			vector teleport_dest = map_widget.ScreenToMap(Vector(x, y, 0));
			vector current_pos = Editor.ActiveCamera.GetPosition();
			teleport_dest[1] = current_pos[1] - GetGame().SurfaceY(current_pos[0], current_pos[2]) + GetGame().SurfaceY(teleport_dest[0], teleport_dest[2]);
			Editor.ActiveCamera.SetPosition(teleport_dest);			
			
			return true;
			
		}
		
		return false;
	}
	
	MapWidget GetMapWidget() { return MapWidget.Cast(m_Root); }
}

class EditorUI: EditorWidgetEventHandler
{	
	
	static ref EditorUI m_Instance;
	static EditorUI GetInstance() { return m_Instance; }
	
	// Canvas
	static ref CanvasWidget EditorCanvas;
	
	// Buttons
	protected ButtonWidget m_BuildingSelect;
	protected ButtonWidget m_VehicleSelect;
	protected ButtonWidget m_EntitySelect;
	protected ButtonWidget m_HumanSelect;
	protected ButtonWidget m_LeftbarHide;
	protected ButtonWidget m_RightbarHide;
	protected ButtonWidget m_ToolbarUndo;
	protected ButtonWidget m_ToolbarRedo;
	protected ButtonWidget m_ToolbarMagnet;
	
	// Frames and Hosts
	protected Widget m_LeftbarFrame;
	protected Widget m_RightbarFrame;
	protected Widget m_LeftbarPanelHost;
	protected Widget m_RightbarPanelHost;
	

	// Map
	protected Widget 			m_EditorMapContainer;
	protected MapWidget 		m_EditorMapWidget;
	protected ref EditorMap 	m_EditorMap;

	// Spacers for Item Lists
	protected WrapSpacerWidget 	m_LeftbarSpacer;
	protected WrapSpacerWidget 	m_RightbarSpacer;
	
	// Debug
	TextWidget m_DebugText1;
	TextWidget m_DebugText2;
	TextWidget m_DebugText3;
	TextWidget m_DebugText4;
	TextWidget m_DebugText5;
	TextWidget m_DebugText6;
	
	static ref array<string>				EditorPlaceableObjects;
	

	// make it so you can search for items by mod name with @ModNameHere
	void EditorUI()
	{
		

		
		m_Instance = this;
	}
	
	void ~EditorUI()
	{
		
	}

	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("EditorUI::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		// Canvas
		EditorCanvas			= CanvasWidget.Cast(m_Root.FindAnyWidget("EditorCanvas"));
		
		
		// Frames
		m_LeftbarFrame			= m_Root.FindAnyWidget("LeftbarFrame");
		m_RightbarFrame			= m_Root.FindAnyWidget("RightbarFrame");
		m_LeftbarPanelHost		= m_Root.FindAnyWidget("LeftbarPanelHost");
		m_RightbarPanelHost		= m_Root.FindAnyWidget("RightbarPanelHost");
		
		
		m_EditorMapContainer	= m_Root.FindAnyWidget("MapContainer");
		m_EditorMapWidget		= MapWidget.Cast(m_Root.FindAnyWidget("Map"));
		
 
		// Buttons
		m_BuildingSelect		= ButtonWidget.Cast(m_Root.FindAnyWidget("BuildingSelect"));
		m_VehicleSelect			= ButtonWidget.Cast(m_Root.FindAnyWidget("VehicleSelect"));
		m_EntitySelect			= ButtonWidget.Cast(m_Root.FindAnyWidget("EntitySelect"));
		m_HumanSelect			= ButtonWidget.Cast(m_Root.FindAnyWidget("HumanSelect"));
		m_LeftbarHide			= ButtonWidget.Cast(m_Root.FindAnyWidget("LeftbarHide"));
		m_RightbarHide			= ButtonWidget.Cast(m_Root.FindAnyWidget("RightbarHide"));
		m_ToolbarUndo			= ButtonWidget.Cast(m_Root.FindAnyWidget("ToolbarUndo"));
		m_ToolbarRedo			= ButtonWidget.Cast(m_Root.FindAnyWidget("ToolbarRedo"));
		m_ToolbarMagnet			= ButtonWidget.Cast(m_Root.FindAnyWidget("ToolbarMagnet"));
		
		// Spacers
		m_LeftbarSpacer			= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("LeftbarSpacer"));
		m_RightbarSpacer		= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("RightbarSpacer"));
		
		// Debug
		m_DebugText1			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText1"));
		m_DebugText2			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText2"));
		m_DebugText3			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText3"));
		m_DebugText4			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText4"));
		m_DebugText5			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText5"));
		m_DebugText6			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText6"));
		
		// Enumerate Placeable Objects
		LoadPlaceableObjects();
		foreach (string placeable_object: EditorPlaceableObjects) {
			EditorListItem list_item;
			Widget list_widget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorListItem.layout", m_LeftbarSpacer);
			list_widget.GetScript(list_item);
			list_item.SetObject(placeable_object);
		}
		
		// Event subscriptions
		EditorEvents.OnObjectCreated.Insert(OnObjectCreated);
		EditorEvents.OnObjectDrag.Insert(HandleObjectDrag);
		EditorEvents.OnObjectDrop.Insert(HandleObjectDrop);
				
		// Update Thread
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	private bool left_bar_hidden = false;
	private bool right_bar_hidden = false;
	
	
	void Update()
	{
		m_DebugText1.SetText(string.Format("X: %1 Y: %2 Z: %3", Editor.CurrentMousePosition[0], Editor.CurrentMousePosition[1], Editor.CurrentMousePosition[2]));
		
		string line1;
		if (!Editor.EditorObjectUnderCursor) 
			line1 = "NULL";
		else 
			line1 = Editor.EditorObjectUnderCursor.GetType();
		m_DebugText2.SetText(line1);
		m_DebugText3.SetText(Editor.SelectedObjects.Count().ToString());
		
		if (Editor.IsPlacing() && IsMapOpen()) {
			
		}
		
	}
	
	void LoadPlaceableObjects()
	{
		Print("EditorUI::LoadPlaceableObjects");
		EditorPlaceableObjects = new array<string>();
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);

		
		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < g_Game.ConfigGetChildrenCount(Config_Path); j++) {
				string Config_Name, Base_Name;
		        GetGame().ConfigGetChildName(Config_Path, j, Config_Name);
		        GetGame().ConfigGetBaseName(Config_Path + " " + Config_Name, Base_Name);
		        Base_Name.ToLower();
		
		        if (Base_Name != "housenodestruct")
		            continue;
			        
				EditorPlaceableObjects.Insert(Config_Name);	
				
		    }
		}
	}
	

	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorUI::OnClick");
		if (button == 0) {
			if (w == m_ToolbarMagnet) {
				MAGNET_PLACEMENT = m_ToolbarMagnet.GetState();
				return true;
			}
		}
		
		return false;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorUI::OnMouseButtonDown: " + button);
		Input input = GetGame().GetInput();	
		// Left Click
		if (button == 0) {
			
			if (w == m_LeftbarHide) {
				if (left_bar_hidden) {
					m_LeftbarFrame.SetPos(0, 48);
					left_bar_hidden = false;
				} else {
					m_LeftbarFrame.SetPos(-300, 48);
					left_bar_hidden = true;
				}
				return true;
			} 
			
			if (w == m_RightbarHide) {
				if (right_bar_hidden) {
					m_RightbarFrame.SetPos(0, 48);
					right_bar_hidden = false;
				} else {
					m_RightbarFrame.SetPos(-300, 48);
					right_bar_hidden = true;
				}
				return true;
			}
			
			if (Editor.IsPlacing()) {
				
				EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
				vector mat[4];
				e.GetTransform(mat);
				EditorObject editor_object = Editor.CreateObject(e.GetType(), mat);
				editor_object.Select();
				if (!input.LocalValue("UATurbo")) delete Editor.ObjectInHand;
				
			} else if (Editor.EditorObjectUnderCursor == null) {
				// delayed dragbox
				EditorUI.EditorCanvas.Clear();
				GetCursorPos(start_x, start_y);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DelayedDragBoxCheck, 40);
				
			} else if (Editor.EditorObjectUnderCursor != null) {
				Editor.EditorObjectUnderCursor.Select(!input.LocalValue("UATurbo"));
			}
			
		}
		
		// Right Click
		if (button == 1) {

		}

		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		Print("EditorUI::OnMouseButtonUp: " + button);
		
		if (button == 0) {
			if (IsDragging) { 
				EditorUI.EditorCanvas.Clear();
				DragBoxQueue.Remove(UpdateDragBox);
				IsDragging = false;
			}
			
		}
		
		return false;
		
	}
	
	bool OnKeyPress(int key)
	{
		switch (key) {
			
			case KeyCode.KC_M: {
				
				if (m_EditorMapContainer.IsVisible()) {
					CloseMap();
				} else {
					OpenMap();
				}
				m_EditorMapContainer.Update();
				return true;
			}
		
			case KeyCode.KC_SPACE: {
				if (GetGame().GetUIManager().IsCursorVisible() && !m_EditorMapContainer.IsVisible()) 
					HideCursor();
				else ShowCursor();
				return true;
			}
			
			case KeyCode.KC_U: {
				MAGNET_PLACEMENT = !MAGNET_PLACEMENT;
				m_ToolbarMagnet.SetState(MAGNET_PLACEMENT);
				m_ToolbarMagnet.Update();
				return true;
			}
			
			case KeyCode.KC_Y: {
				m_Root.Show(!m_Root.IsVisible());
				
				return true;
			}
		}
		return false;
	}
	
	ScriptInvoker DragBoxQueue = GetGame().GetUpdateQueue(CALL_CATEGORY_GUI);
	int start_x, start_y;
	void DelayedDragBoxCheck()
	{
		Input input = GetGame().GetInput();
		if (input.LocalValue("UAFire")) {
			DragBoxQueue.Insert(UpdateDragBox);
		}
		
	}
	
	bool IsDragging = false; // this is very broken find a better way to do this
	void UpdateDragBox()
	{	
		IsDragging = true;
		int current_x, current_y;
		GetCursorPos(current_x, current_y);
		EditorUI.EditorCanvas.Clear();
		int selection_box_thickness = 2;
		int selection_box_color = ARGB(100, 255, 0, 0);
		EditorUI.EditorCanvas.DrawLine(start_x, start_y, current_x, start_y, selection_box_thickness, selection_box_color);
		EditorUI.EditorCanvas.DrawLine(start_x, start_y, start_x, current_y, selection_box_thickness, selection_box_color);
		EditorUI.EditorCanvas.DrawLine(start_x, current_y, current_x, current_y, selection_box_thickness, selection_box_color);
		EditorUI.EditorCanvas.DrawLine(current_x, start_y, current_x, current_y, selection_box_thickness, selection_box_color);
		
		int x_low, x_high, y_low, y_high;
		
		
		if (start_x > current_x) {
			x_high = start_x;
			x_low = current_x;
		} else { 
			x_high = current_x;
			x_low = start_x;
		}
		
		if (start_y > current_y) {
			y_high = start_y;
			y_low = current_y;
		} else { 
			y_high = current_y;
			y_low = start_y;
		}
		
		
		foreach (EditorObject editor_object: Editor.EditorObjects) {
			
			float marker_x, marker_y;
			if (IsMapOpen()) {
				editor_object.GetMapMarker().GetPos(marker_x, marker_y);
			} else {
				editor_object.GetObjectMarker().GetPos(marker_x, marker_y);
			}
			if ((marker_x < x_high && marker_x > x_low) && (marker_y < y_high && marker_y > y_low)) {				
				editor_object.Select(false);
			} else {
				editor_object.Deselect();
			}
		}
		
		
	}
	
	
	void OpenMap()
	{
		Print("EditorUI::OpenMap");
		m_LeftbarPanelHost.SetAlpha(m_LeftbarPanelHost.GetAlpha() * 3);
		m_RightbarPanelHost.SetAlpha(m_RightbarPanelHost.GetAlpha() * 3);
		m_EditorMapContainer.Show(true);
		ShowCursor();
	}
	
	void CloseMap()
	{
		Print("EditorUI::CloseMap");
		m_LeftbarPanelHost.SetAlpha(m_LeftbarPanelHost.GetAlpha() / 3);
		m_RightbarPanelHost.SetAlpha(m_RightbarPanelHost.GetAlpha() / 3);
		m_EditorMapContainer.Show(false);
		ShowCursor();
	}
	
	MapWidget GetMapWidget() { return m_EditorMapWidget; }
	
	bool IsMapOpen() { return m_EditorMapContainer.IsVisible(); }
	
	void ShowCursor()
	{
		EditorCamera.LookFreeze = true;
		GetGame().GetUIManager().ShowUICursor(true);
	}
	
	void HideCursor()
	{
		EditorCamera.LookFreeze = false;
		GetGame().GetUIManager().ShowUICursor(false);
	}
	
	void ShowExportWindow()
	{
		Print("EditorUI::ShowExportWindow");	
		
		//GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorExportWindow.layout", Getm_Root());
		
		EditorExportWindow dialog = new EditorExportWindow();
		//GetGame().GetUIManager().ShowScriptedMenu(dialog, this);
	}
	
	void OnObjectCreated(Class context, EditorObject target)
	{
		m_RightbarSpacer.AddChild(target.GetObjectBrowser());
	}
	
	
	void HandleObjectDrag(Class context, EditorObject target)
	{
		target.Select(false); // sanity check
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ObjectDragUpdate, 0, true, target);		
	}
	
	void HandleObjectDrop(Class context, EditorObject target)
	{		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(ObjectDragUpdate);
	}
	
	
	void ObjectDragUpdate(EditorObject target)
	{	
		
		Object target_object = target.GetObject();
		vector object_position = target_object.GetPosition();
		vector object_size = target.GetSize();
		vector object_orientation = target_object.GetOrientation();

		set<Object> obj;
		vector cursor_position = MousePosToRay(obj, target_object);
		cursor_position[1] = cursor_position[1] + object_size[1]/2;
		
		Input input = GetGame().GetInput();
		
		
		// If map is ON	
		if (IsMapOpen()) {
			
			int mouse_x, mouse_y;
			GetCursorPos(mouse_x, mouse_y);
			cursor_position = m_EditorMapWidget.ScreenToMap(Vector(mouse_x, mouse_y, 0));
			cursor_position[1] = object_position[1] - GetGame().SurfaceY(object_position[0], object_position[2]) + GetGame().SurfaceY(cursor_position[0], cursor_position[2]);
			target.SetPosition(cursor_position);
			
		} else {
		
			// Handle Z only motion*
			if (input.LocalValue("UALookAround")) {	
				//dist_to_ground = object_position[1] - GetGame().SurfaceY(object_position[0], object_position[2]);
				float dist = vector.Distance(GetGame().GetCurrentCameraPosition(), object_position);
				cursor_position = MousePosToRay(obj, target_object, dist);
				cursor_position[0] = object_position[0];
				cursor_position[1] = cursor_position[1] + object_size[1]/2;
				cursor_position[2] = object_position[2];
				target.SetPosition(cursor_position);
			
			// Handle XY Rotation
			} else if (input.LocalValue("UATurbo")) {
				vector cursor_delta = cursor_position - object_position;
				float angle = Math.Atan2(cursor_delta[0], cursor_delta[2]);
				object_orientation[0] = angle * Math.RAD2DEG;
				
				target.SetOrientation(object_orientation);
				
			} else {
				

				vector mat[4] = { "1 0 0", "0 1 0", "0 0 1", cursor_position };
				vector surface_normal = GetGame().SurfaceGetNormal(cursor_position[0], cursor_position[2]);
				target.PlaceOnSurfaceRotated(mat, cursor_position, surface_normal[0] * -1, surface_normal[2] * -1, 0, MAGNET_PLACEMENT);
				target.SetTransform(mat);
					
				
			}
		}
			
		target.Update();
					
		// This handles all other selected objects
		foreach (EditorObject selected_object: Editor.SelectedObjects) {
			
			if (selected_object == target) continue;
			vector selected_size = selected_object.GetSize();
			vector selected_position = selected_object.GetPosition();
			vector pos_delta = selected_position - object_position;
			
			// If map is ON	
			if (IsMapOpen()) {
			
				cursor_position[1] = object_position[1] - GetGame().SurfaceY(object_position[0], object_position[2]) + GetGame().SurfaceY(cursor_position[0], cursor_position[2]);
				selected_object.SetPosition(cursor_position + pos_delta); 
				
			} else {
				
				if (input.LocalValue("UALookAround")) {
					pos_delta[0] = selected_position[0];
					pos_delta[1] = cursor_position[1] + pos_delta[1];
					pos_delta[2] = selected_position[2];
					selected_object.SetPosition(pos_delta);
					
				} else if (input.LocalValue("UATurbo")) {
					vector rot_pos;
					float magnitude = Math.Sqrt(Math.Pow(selected_position[0] - object_position[0], 2) + Math.Pow(selected_position[2] - object_position[2], 2));
					//Print(magnitude);
					
					vector normal_delta = pos_delta.Normalized();
					float delta_angle = Math.Atan2(normal_delta[0], normal_delta[2]);
					//Print(delta_angle);
					float ang = (delta_angle + angle) * Math.RAD2DEG;
					
					
					
					float x = ((selected_position[0] - object_position[0]) * Math.Cos(delta_angle)) - ((object_position[2] - selected_position[2]) * Math.Sin(delta_angle)) + object_position[0];
					float y = ((object_position[2] - selected_position[2]) * Math.Cos(delta_angle)) - ((selected_position[0] - object_position[0]) * Math.Sin(delta_angle)) + object_position[2];
					
					rot_pos[0] = x;
					rot_pos[1] = selected_position[1];
					rot_pos[2] = y;
					Print(rot_pos);
					
					selected_object.SetPosition(rot_pos);
					
				} else {
					
					selected_object.SetPosition(cursor_position + pos_delta); 			
				}	
			}
			
			selected_object.Update();
		}
	}
	
	
	
}