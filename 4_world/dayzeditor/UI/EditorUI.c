
// make it so you can search for items by mod name with @ModNameHere




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
	protected ButtonWidget m_ToolbarGround;
	
	// Frames and Hosts
	protected Widget m_LeftbarFrame;
	protected Widget m_RightbarFrame;
	protected Widget m_LeftbarPanelHost;
	protected Widget m_RightbarPanelHost;
	

	// Misc
	protected Widget 			m_EditorMapContainer;
	protected MapWidget 		m_EditorMapWidget;
	protected ref EditorMap 	m_EditorMap;
	protected EditBoxWidget 	m_LeftbarSearchBar;	

	// Spacers for Item Lists
	protected WrapSpacerWidget 	m_LeftbarSpacer;
	protected WrapSpacerWidget 	m_RightbarSpacer;
	
	// Orientation Tool
	protected ItemPreviewWidget m_OrientationWidget;
	
	// Debug
	TextWidget 			m_DebugText1;
	TextWidget			m_DebugText2;
	TextWidget 			m_DebugText3;
	TextWidget 			m_DebugText4;
	TextWidget 			m_DebugText5;
	TextWidget 			m_DebugText6;
	TextListboxWidget 	m_DebugActionStack;
	
	

	
	void EditorUI()
	{
		m_Instance = this;
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
		m_OrientationWidget		= ItemPreviewWidget.Cast(m_Root.FindAnyWidget("OrientationView"));
		
		// Misc
		m_EditorMapContainer	= m_Root.FindAnyWidget("MapContainer");
		m_EditorMapWidget		= MapWidget.Cast(m_Root.FindAnyWidget("Map"));
		m_LeftbarSearchBar		= EditBoxWidget.Cast(m_Root.FindAnyWidget("LeftbarSearchBar"));
 
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
		m_ToolbarGround			= ButtonWidget.Cast(m_Root.FindAnyWidget("ToolbarGround"));
		
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
		m_DebugActionStack		= TextListboxWidget.Cast(m_Root.FindAnyWidget("DebugActionStackListbox"));
		
		//EntityAI translate = GetGame().CreateObject("TranslationWidget", vector.Zero);
		
		//m_OrientationWidget.SetItem(translate);
		m_EditorMapWidget.GetScript(m_EditorMap);
		m_EditorMapWidget.SetMapPos(GetGame().GetCurrentCameraPosition());
		
	}
	
	private bool left_bar_hidden = false;
	private bool right_bar_hidden = false;
	
	
	

	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorUI::OnClick");
		if (button == 0) {
			if (w == m_ToolbarMagnet) {
				EditorSettings.MAGNET_PLACEMENT = m_ToolbarMagnet.GetState();
				return true;
			}
			if (w == m_ToolbarGround) {
				EditorSettings.MAINTAIN_HEIGHT = m_ToolbarGround.GetState();
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
				left_bar_hidden = !left_bar_hidden;
				m_RightbarFrame.SetPos(-250 * left_bar_hidden, 48);
				return true;
			} 
			
			if (w == m_RightbarHide) {
				right_bar_hidden = !right_bar_hidden;
				m_RightbarFrame.SetPos(-250 * right_bar_hidden, 48);
				return true;
			}
			
			if (Editor.IsPlacing()) {
				Editor.PlaceObject();
				return true;
				
			} else if (Editor.GlobalTranslationWidget.IsMouseInside()) 
				return true; 
			else if (Editor.EditorObjectUnderCursor == null) {
				// delayed dragbox
				EditorUI.EditorCanvas.Clear();
				GetCursorPos(start_x, start_y);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DelayedDragBoxCheck, 60);
				
			} else if (Editor.EditorObjectUnderCursor != null) {
				Editor.EditorObjectUnderCursor.Select(!input.LocalValue("UATurbo"));
			}
			
			
		}
		
		// Middle Mouse
		if (button == 2) {
			if (input.LocalValue("UAWalkRunTemp")) {
				set<Object> o;
				vector pos = MousePosToRay(o);
				LightingBolt.CreateLightning(pos, 5);
			}
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
				EditorSettings.MAGNET_PLACEMENT = !EditorSettings.MAGNET_PLACEMENT;
				m_ToolbarMagnet.SetState(EditorSettings.MAGNET_PLACEMENT);
				m_ToolbarMagnet.Update();
				SetFocus(null);
				return true;
			}
			
			case KeyCode.KC_Y: {
				m_Root.Show(!m_Root.IsVisible());
				foreach (EditorObject editor_object: Editor.PlacedObjects) {
					editor_object.GetObjectMarker().Show(m_Root.IsVisible());
				}
				
				return true;
			}
			
			case KeyCode.KC_G: {
				
				EditorSettings.MAINTAIN_HEIGHT = !EditorSettings.MAINTAIN_HEIGHT;
				m_ToolbarGround.SetState(EditorSettings.MAINTAIN_HEIGHT);
				m_ToolbarGround.Update();
				SetFocus(null);
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
		Input input = GetGame().GetInput();
		if (input.LocalRelease("UAFire")) {
			DragBoxQueue.Remove(UpdateDragBox);
		}
		
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
		
		
		foreach (EditorObject editor_object: Editor.PlacedObjects) {
			
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
	
	EditorMap GetMap() { return m_EditorMap; }
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
		GetGame().GetUIManager().ShowScriptedMenu(dialog, GetGame().GetUIManager().GetMenu());
	}
	
	
	void InsertPlaceableObject(string placeable_object)
	{
		EditorListItem list_item;
		Widget list_widget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorListItem.layout", m_LeftbarSpacer);
		list_widget.GetScript(list_item);
		list_item.SetObject(placeable_object);
	}
	
	void InsertPlacedObject(EditorObject target)
	{
		m_RightbarSpacer.AddChild(target.GetObjectBrowser());
	}
	
	
	
	
	
}