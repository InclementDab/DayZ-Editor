
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
		
		monitor = new DebugMonitor();
		monitor.Init();
		
		return layoutRoot;
	}
	
	int x1, y1;
	vector mouse_start;
	void Update(float timeslice)
	{
		monitor.SetPosition(GetGame().GetPointerDirection());
		Input input = GetGame().GetInput();
		
		if (input.LocalPress("UAFire")) {
			GetMousePos(x1, y1);
			
			vector start1 = GetGame().GetCurrentCameraPosition();
			vector end1 = start1 + GetGame().GetPointerDirection() * 100;
			vector dir1;
			int component;
			
			DayZPhysics.RaycastRV(start1, end1, mouse_start, dir1, component);
		}
		
		if (input.LocalHold("UAFire")) {
			int x2, y2;
			GetMousePos(x2, y2);
			Debug.ClearCanvas();
			Debug.m_CanvasDebug.DrawLine(x1, y1, x2, y1, 2, COLOR_BLUE);
			Debug.m_CanvasDebug.DrawLine(x1, y1, x1, y2, 2, COLOR_BLUE);
			Debug.m_CanvasDebug.DrawLine(x1, y2, x2, y2, 2, COLOR_BLUE);
			Debug.m_CanvasDebug.DrawLine(x2, y1, x2, y2, 2, COLOR_BLUE);

			vector direction = GetGame().GetCurrentCameraDirection();
			vector start = GetGame().GetCurrentCameraPosition();
			vector end = start + GetGame().GetPointerDirection() * 100;
			vector pos, dir;
			
			DayZPhysics.RaycastRV(start, end, pos, dir, component);
			float x3 = Math.AbsFloat(mouse_start[0] - pos[0]);
			float y3 = 1000;
			float z3 = Math.AbsFloat(mouse_start[2] - pos[2]);
			vector edgelength = {x3, y3, z3};
			array<Object> excluded = new array<Object>;
			array<Object> results = new array<Object>;
			
			GetGame().IsBoxColliding(start, direction, edgelength, excluded, results);
			
			monitor.SetBlood(results.Count());
		}
		
		if (input.LocalRelease("UAFire")) {
			Debug.ClearCanvas();
		}
		
		
		
		
		super.Update(timeslice);
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
					Editor.SelectObject(Editor.ObjectUnderCursor);
					return true;
			}	
		}
		return false;
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






