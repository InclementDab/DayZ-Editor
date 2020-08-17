

static const float ALPHA_ON_SHOW = 1;
static const float ALPHA_ON_HIDE = 0.25;


class EditorMarker: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() { return m_LayoutRoot; }

	void EditorMarker()
	{
		EditorLog.Trace("EditorMarker");

		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorMarker.layout");
		m_LayoutRoot.SetHandler(this);
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorMarker()
	{
		EditorLog.Trace("~EditorMarker");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		m_LayoutRoot.Unlink();
	}
	
	
	void Update() {	}
	
	void SetPos(float x, float y) 
	{
		// Offset to center of marker
		x -= 15; y -= 15;
		m_LayoutRoot.SetPos(x, y);
		m_LayoutRoot.Update();
	}
	
	void GetPos(out float x, out float y)
	{
		m_LayoutRoot.GetPos(x, y);
	}
	
	void Show(bool show)
	{
		m_LayoutRoot.Show(show);
	}
	
	protected bool IsMouseInside(int c_x, int c_y)
	{
		float x, y, w, h;
		m_LayoutRoot.GetPos(x, y);
		m_LayoutRoot.GetSize(w, h);
		return (c_x < x + h && c_x > x - h) && (c_y < y + h && c_y > y - h);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorMarker::OnMouseEnter");
		// set cursor here?
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		EditorLog.Trace("EditorMarker::OnMouseLeave");
		return true;
	}

	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorMarker::OnDrag");
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorMarker::OnDrop");
		SetPos(x, y);
		return true;
	}
}

class EditorObjectMarker: EditorMarker
{	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	void EditorObjectMarker(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectMarker");
		m_EditorObject = editor_object;
	}
	
	override void Update()
	{
		int x, y;
		GetCursorPos(x, y);
		if (m_EditorObject.IsSelected() || IsMouseInside(x, y)) 
			m_LayoutRoot.SetAlpha(ALPHA_ON_SHOW);
		else 
			m_LayoutRoot.SetAlpha(ALPHA_ON_HIDE);
		
		super.Update();
	}
}


class EditorObjectMapMarker: EditorObjectMarker
{		
	private MapWidget m_MapWidget;
	MapWidget GetMapWidget() { return m_MapWidget; }
	
	
	override void Update()
	{
		m_MapWidget = MapWidget.Cast(m_LayoutRoot.GetParent());
		vector pos = m_MapWidget.MapToScreen(m_EditorObject.GetPosition());	
		
	
		super.Update();
	}
	
		

	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorMapMarker::OnMouseButtonDown");
		
		Input input = GetGame().GetInput();
		
		// ignores the object if you are placing
		if (GetEditor().IsPlacing()) return false;
		
		// allows multiple objects to be dragged
		if (m_EditorObject.IsSelected()) 
			return true;
		
		// We want to Toggle selection if you are holding control
		if (input.LocalValue("UARunWalkTemp")) {
			GetEditor().ToggleSelection(m_EditorObject);
		} else {
			if (!input.LocalValue("UATurbo"))
				GetEditor().ClearSelection();
			
			GetEditor().SelectObject(m_EditorObject);
		}
		 // Blocks map from creating selection box
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorMapMarker::OnDrag");
		if (GetEditor().IsPlacing()) return false;
		//EditorEvents.DragInvoke(this, m_EditorObject);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		Print("EditorMapMarker::OnDrop");	
		//EditorEvents.DropInvoke(this, m_EditorObject);
		return true;
	}
}

class EditorObjectWorldMarker: EditorObjectMarker
{
	void EditorObjectWorldMarker(EditorObject editor_object)
	{
		EditorEvents.OnMapToggled.Insert(OnMapToggled);
	}
	
	override void Update()
	{
		vector position;
		vector object_transform[4];
		m_EditorObject.GetTransform(object_transform);
		
		// Should the position be raycasted on the ground, or locked to the object
		if (EditorSettings.MAINTAIN_HEIGHT) {
			set<Object> o;
			vector ground_dir; int component;
			DayZPhysics.RaycastRV(object_transform[3], object_transform[3] + object_transform[1] * -1000, position, ground_dir, component, o, NULL, m_EditorObject.GetWorldObject(), false, true); // set to ground only
			
		} else position = m_EditorObject.GetBottomCenter();
	
		vector screen_pos = GetGame().GetScreenPos(position);
		SetPos(screen_pos[0], screen_pos[1]);
		
		super.Update();
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorObjectWorldMarker::OnClick: " + button);
		
		if (button == 0) {
			
			if (GetEditor().IsPlacing()) return false;
			
			// We want to Toggle selection if you are holding control
			if (KeyState(KeyCode.KC_LCONTROL))
				GetEditor().ToggleSelection(m_EditorObject);
			else {
				// required for multiple objects to be dragged
				if (m_EditorObject.IsSelected()) 
					return true;
				
				if (!KeyState(KeyCode.KC_LSHIFT))
					GetEditor().ClearSelection();
				
				GetEditor().SelectObject(m_EditorObject);
			}	
		}
		
		return super.OnClick(w, x, y, button);
	}

	
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorObjectWorldMarker::OnDrag");
		if (GetEditor().IsPlacing()) return false;
		
		GetEditor().SelectObject(m_EditorObject);
		
		return super.OnDrag(w, x, y);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorObjectWorldMarker::OnDrop");
		
		return super.OnDrop(w, x, y, reciever);
	}
	

	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{		
			/*
		m_EditorObjectPropertiesWindow = new UILinkedObject();
		m_EditorObjectPropertiesWidget = g_Game.GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorObjectProperties.layout");
		m_EditorObjectPropertiesWidget.GetScript(m_EditorObjectPropertiesWindow);
		m_EditorObjectPropertiesWindow.SetObject(this);
		m_EditorObjectPropertiesWidget.Show(false);		
		*/
		return true;
	}

	void OnMapToggled(Class context, EditorMap editor_map, bool state)
	{
		
	}
	

}











