
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


class EditorUI: UIScriptedMenu
{
	static ref EditorUI m_Instance;
	static EditorUI GetInstance()
	{
		return m_Instance;
	}
	
	protected Widget m_Parent;
	
	protected Widget m_EditorUIFrame;
	protected Widget m_LeftListPanel;
	protected Widget m_RightListPanel;
	
	protected EditBoxWidget m_LeftSearchBar;
	
	protected ScrollWidget m_LeftListPanelScrollBar;
	WrapSpacerWidget m_LeftListPanelSpacer;	
	
	protected ScrollWidget m_RightListPanelScrollBar;
	protected WrapSpacerWidget m_RightListPanelSpacer;
		
	protected ref EditorSearchBar m_EditorSearchBar;
	
		
	
	void EditorUI()
	{
		Print("EditorUI");
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
		
		
		
		//monitor = new DebugMonitor();
		//monitor.Init();
		
		return layoutRoot;
	}
	
	
	void OnDrag(int x, int y)
	{
		Print("OnDrag");
		start_x = x; 
		start_y = y;

		
		//GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(OnDragging);
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
				
		foreach (EditorObject list_item: Editor.EditorPlacedObjects) {
			float pos_x, pos_y;
			list_item.GetMarkerPosition(pos_x, pos_y);
			if ((pos_x < x_high && pos_x > x_low) && (pos_y < y_high && pos_y > y_low)) {
				Editor.CreateSelection(list_item.WorldObject, false);
			}	
		}
		
		
		
		if (!_is_dragging) {
			Debug.ClearCanvas();
		}
		
	}
		
	
	
	void Show(bool state)
	{
		layoutRoot.Show(state);
	}

	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print(w.GetName());
		

		Print("EditorUI::OnMouseButtonDown");
		
		if (button == 0) {
							
			if (w == GetFocus()) return true;
			if (w.GetName() == "EditorListItemFrame") { // idk why but i cant use OnMouseButtonDown in editorobject
				if (Editor.IsPlacing()) delete Editor.ObjectInHand;
				SetFocus(w.GetParent());
				return true;
			} else if (Editor.IsPlacing()) {
				EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
				Editor.PlaceObject(e.GetType(), e.GetWorldPosition(), vector.Up);				
	
				if (!GetGame().GetInput().LocalValue("UATurbo")) {
					delete Editor.ObjectInHand;
				} 
				return true;
				
			} else if (Editor.ObjectUnderCursor != null) {
				//return false; // todo: check if object under cursor is one we placed
				return true;
			} else if (!is_dragging) {
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CallUpdateDrag, 40);
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
	
	
	

	

		
		
	void OnObjectPlaced(Object obj, vector position, vector orientation)
	{
		Print("EditorUI::OnObjectPlaced");
				
		EditorObject editor_object;
		Widget editor_object_display = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorObjectMarker.layout");
		editor_object_display.GetScript(editor_object);
		m_RightListPanelSpacer.AddChild(editor_object.Initialize(obj));
		Editor.EditorPlacedObjects.Insert(editor_object);
	}

	EditorObject CreateEditorObjectFromExisting(Object obj)
	{		
		Print("EditorUI::CreateEditorObjectFromExisting");
		EditorObject editor_object;
		Widget editor_object_display = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorObjectMarker.layout");
		editor_object_display.GetScript(editor_object);
		m_RightListPanelSpacer.AddChild(editor_object.Initialize(obj));
		Editor.EditorPlacedObjects.Insert(editor_object);
		return editor_object;
	}
}






