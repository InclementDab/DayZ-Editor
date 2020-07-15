
vector ScreenPosToRay()
{
	int x, y;
	GetMousePos(x, y);
	
	int x1, y1;
	GetScreenSize(x1, y1);
	int x2 = x1 / x; 
	int y2 = y1 / y;
	
	vector s = GetGame().GetCurrentCameraPosition();
	vector p = { x2, 0, y2 };
	vector r = { 5, 5, 5 };
	
	
	vector result = DivideVector(s + r * (p-s), (p-s).Normalized());
	
	
	
	
	return result;
}


modded class EditorUI
{

	
	protected Widget m_Parent;
	
	protected Widget m_EditorUIFrame;
	protected Widget m_LeftListPanel;
	protected Widget m_RightListPanel;
	
	protected EditBoxWidget m_LeftSearchBar;
	
	protected ScrollWidget m_LeftListPanelScrollBar;
	WrapSpacerWidget m_LeftListPanelSpacer;	
	

	

	protected ScrollWidget m_RightListPanelScrollBar;
	WrapSpacerWidget m_RightListPanelSpacer;
	
	override WrapSpacerWidget GetRightPanelSpacer()
	{
		return m_RightListPanelSpacer;
	}
		
	protected ref EditorSearchBar m_EditorSearchBar;
	
	static ref array<EditorListItem>	EditorListItems;
	
	EditorCamera							m_Camera;
		
	
	void EditorUI()
	{
		Print("EditorUI");
		EditorListItems = new array<EditorListItem>();
						
		float pos1 = Math.RandomFloat(2500, 13000);
		float pos2 = Math.RandomFloat(2500, 13000);
		m_Camera = GetGame().CreateObject("EditorCamera", Vector(pos1, 500, pos2), false);
		m_Camera.SetActive(true);
	}
	
	void ~EditorUI()
	{
		Print("~EditorUI");
	}
	
	ref DebugMonitor monitor;
	override Widget Init()
	{
		Print("EditorUI::Init");
		layoutRoot 					= GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorUI.layout", m_Parent);
				
		m_EditorUIFrame 			= layoutRoot.FindAnyWidget("EditorUIFrame");
		m_LeftListPanel 			= layoutRoot.FindAnyWidget("LeftListPanel");
		m_RightListPanel 			= layoutRoot.FindAnyWidget("RightListPanel");

		m_LeftSearchBar 			= EditBoxWidget.Cast(layoutRoot.FindAnyWidget("LeftPanelSearchBar"));		
		m_LeftListPanelScrollBar 	= ScrollWidget.Cast(layoutRoot.FindAnyWidget("LeftPanelScrollBar"));
		m_LeftListPanelSpacer 		= WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("LeftListPanelSpacer"));		
		m_RightListPanelScrollBar 	= ScrollWidget.Cast(layoutRoot.FindAnyWidget("RightPanelScrollBar"));
		m_RightListPanelSpacer 		= WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("RightListPanelSpacer"));

		m_EditorSearchBar = new EditorSearchBar(m_LeftListPanelSpacer);
		m_LeftSearchBar.SetHandler(m_EditorSearchBar);
		
		Debug.InitCanvas();	
		
		layoutRoot.Show(true);
		m_Instance = this;
		
		foreach (string item_name: Editor.EditorPlaceableObjects) {
			EditorListItem editor_list_item;
			Widget list_item = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorListItem.layout", m_LeftListPanelSpacer);
			list_item.GetScript(editor_list_item);
			editor_list_item.SetObject(item_name);
			EditorListItems.Insert(editor_list_item);
		}

		//monitor = new DebugMonitor();
		//monitor.Init();
		
		return layoutRoot;
	}
	
	void Show()
	{
		m_Camera.LookFreeze = true;
		GetGame().GetUIManager().ShowUICursor(true);
	}
	
	void Hide()
	{
		m_Camera.LookFreeze = false;
		GetGame().GetUIManager().ShowUICursor(false);
	}
	
	
	void OnDrag(int x, int y)
	{
		Print("OnDrag");
		start_x = x; 
		start_y = y;
	}
	
	int start_x, start_y;
	bool is_dragging = false;
	void UpdateDrag(int current_x, int current_y, bool _is_dragging)
	{
		
		if (!is_dragging) {
			start_x = current_x; start_y = current_y;	
			Editor.ClearSelections();
		}
		
		
		is_dragging = _is_dragging;
		
		Debug.ClearCanvas();
		int selection_box_thickness = 1;
		int selection_box_color = ARGB(100, 255, 0, 0);
		Debug.m_CanvasDebug.DrawLine(start_x, start_y, current_x, start_y, selection_box_thickness, selection_box_color);
		Debug.m_CanvasDebug.DrawLine(start_x, start_y, start_x, current_y, selection_box_thickness, selection_box_color);
		Debug.m_CanvasDebug.DrawLine(start_x, current_y, current_x, current_y, selection_box_thickness, selection_box_color);
		Debug.m_CanvasDebug.DrawLine(current_x, start_y, current_x, current_y, selection_box_thickness, selection_box_color);
		
		int x_low, x_high, y_low, y_high;
		
		if (start_x > current_x) {
			x_high = start_x;
			x_low = current_x;
		} else { 
			x_low = start_x;
			x_high = current_x;		
		}
		
		if (start_y > current_y) {
			y_high = start_y;
			y_low = current_y;
		} else { 
			y_low = start_y;
			y_high = current_y;		
		}
				
		foreach (EditorObject editor_object: Editor.EditorObjects) {
			
			
			float pos_x, pos_y;
			editor_object.ObjectMarker.GetMarkerPosition(pos_x, pos_y);
			if ((pos_x < x_high && pos_x > x_low) && (pos_y < y_high && pos_y > y_low)) {
				Editor.CreateSelection(editor_object, false);
			}	
		}
		
		if (!_is_dragging) {
			Debug.ClearCanvas();
		}
		
	}
		
		
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print(w.GetName());
		Print("EditorUI::OnMouseButtonDown");
		
		Input input = GetGame().GetInput();


		
		
		if (button == 0) {
							
			if (w == GetFocus()) return true;
			if (w.GetName() == "EditorListItemFrame") { // idk why but i cant use OnMouseButtonDown in editorobject
				if (Editor.IsPlacing()) delete Editor.ObjectInHand;
				SetFocus(w.GetParent());
				return true;
			} else if (Editor.IsPlacing()) {
				EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
				Editor.PlaceObject(e.GetType(), e.GetWorldPosition(), vector.Up);						
				if (!input.LocalValue("UATurbo")) delete Editor.ObjectInHand;
				return true;
				
			} else if (Editor.ObjectUnderCursor == null) Editor.ClearSelections();
			else if (!is_dragging) GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CallUpdateDrag, 40);
			else {
				EditorObject editor_object = Editor.EditorObjectFromObject(Editor.ObjectUnderCursor);
				if (editor_object != null) 
					if (input.LocalValue("UATurbo")) 
						Editor.CreateSelection(editor_object, false);
					else 
						Editor.CreateSelection(editor_object);
					
				
			}
		}
		return false;
	}
	
	
	void CallUpdateDrag()
	{
		if (GetGame().GetInput().LocalValue("UAFire")) {
			GetGame().GetDragQueue().Call(this, "UpdateDrag");
		}
	}
	
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		Editor.CursorAllowedToSelect = true; // this is stupid
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Editor.CursorAllowedToSelect = false;
		return true;
	}
	
	
	

	

		


	EditorObjectMarker CreateEditorObjectFromExisting(Object obj)
	{		/*
		Print("EditorUI::CreateEditorObjectFromExisting");
		EditorObjectMarker editor_object;
		Widget editor_object_display = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorObjectMarker.layout");
		editor_object_display.GetScript(editor_object);
		m_RightListPanelSpacer.AddChild(editor_object.Initialize(obj));
		Editor.EditorPlacedObjects.Insert(editor_object);
		return editor_object;
	*/
	}
}






