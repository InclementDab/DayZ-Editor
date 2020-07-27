

static const float ALPHA_ON_SHOW = 1;
static const float ALPHA_ON_HIDE = 0.25;

class EditorMapMarker: UILinkedObject
{
	protected ImageWidget m_EditorMapMarkerImage;
	
	void EditorMapMarker()
	{
		Print("EditorMapMarker");
	}
	
	void ~EditorMapMarker()
	{
		Print("~EditorMapMarker");
		delete m_EditorMapMarkerImage;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarker::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		m_EditorMapMarkerImage = ImageWidget.Cast(m_Root.FindAnyWidget("EditorMapMarkerImage"));		
	}
	
	override void Update()
	{

		MapWidget map_widget = MapWidget.Cast(m_Root.GetParent());
		vector pos = map_widget.MapToScreen(m_EditorObject.GetPosition());
		
		// -5 for cursor offset
		// -10 to put cursor on center
		m_Root.SetPos(pos[0] - 15, pos[1] - 15);
		
		if (m_EditorObject.IsSelected() || MouseInside) 
			m_Root.SetAlpha(ALPHA_ON_SHOW);
		else 
			m_Root.SetAlpha(ALPHA_ON_HIDE);

	}
	
	private bool MouseInside = false;
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		Print("EditorMapMarker::OnMouseEnter");
		MouseInside = true;
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Print("EditorMapMarker::OnMouseLeave");
		MouseInside = false;
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorMapMarker::OnMouseButtonDown");
		
		Input input = GetGame().GetInput();
		
		// ignores the object if you are placing
		if (Editor.IsPlacing()) return false;
		
		// allows multiple objects to be dragged
		if (m_EditorObject.IsSelected()) return true;
		
		// basic interaction
		if (input.LocalValue("UATurbo"))
			m_EditorObject.Select(false);
		else if (input.LocalValue("UARunWalkTemp"))
			m_EditorObject.ToggleSelect();
		else
			m_EditorObject.Select();
		 // Blocks map from creating selection box
		return true;
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorMapMarker::OnDrag");
		if (Editor.IsPlacing()) return false;
		EditorEvents.DragInvoke(this, m_EditorObject);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		Print("EditorMapMarker::OnDrop");	
		EditorEvents.DropInvoke(this, m_EditorObject);
		return true;
	}
}

class EditorObjectMarker: UILinkedObject
{

	protected ImageWidget m_EditorObjectMarkerImage;
	protected bool override_show = true;
	
	private bool MouseInside = false;
	
	void ~EditorObjectMarker()
	{
		Print("~EditorObjectMarker");
		delete m_EditorObjectMarkerImage;
	}
	
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarker::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);

		m_EditorObjectMarkerImage = ImageWidget.Cast(m_Root.FindAnyWidget("EditorObjectMarkerImage"));		
	}
	
	
	
	override void Update()
	{
		vector position;
		vector object_transform[4];
		m_EditorObject.GetTransform(object_transform);
		
		if (EditorSettings.MAINTAIN_HEIGHT) {
			// Raycast ground below object
			set<Object> o;
			vector ground_dir; int component;
			DayZPhysics.RaycastRV(object_transform[3], object_transform[3] + object_transform[1] * -1000, position, ground_dir, component, o, NULL, m_EditorObject, false, true); // set to ground only
		} else {
			position = m_EditorObject.GetBottomCenter();
		}
		
		vector screenpos = GetGame().GetScreenPos(position);
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		m_Root.SetPos(screenpos[0] - 15, screenpos[1] - 15);
		
		if (override_show) {
			m_Root.Show(!(screenpos[0] > screen_x || screenpos[1] > screen_y || screenpos[0] <= 0 || screenpos[1] <= 0));
		}
		
		if (m_EditorObject.IsSelected() || MouseInside) {
			m_Root.SetAlpha(ALPHA_ON_SHOW);
		} else {
			m_Root.SetAlpha(ALPHA_ON_HIDE);
		}
		
		
		m_Root.Update();
	}
	
	
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		Print("EditorMarker::OnMouseEnter");
		if (Editor.IsPlacing()) return false;
		MouseInside = true;
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Print("EditorMarker::OnMouseLeave");
		MouseInside = false;
		return true;
	}
	

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorObjectMarker::OnMouseButtonDown: " + button);
		
		
		Input input = GetGame().GetInput();
		
		if (Editor.IsPlacing()) return false;
		
		// allows multiple objects to be dragged
		if (m_EditorObject.IsSelected()) return true;
		
		if (input.LocalValue("UATurbo"))
			m_EditorObject.Select(false);
		else if (input.LocalValue("UARunWalkTemp"))
			m_EditorObject.ToggleSelect();
		else
			m_EditorObject.Select();
		


		return true;
	}

	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorObjectMarker::OnDrag");
		
		if (Editor.IsPlacing()) return false;
		
		EditorEvents.DragInvoke(this, m_EditorObject);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		Print("EditorObjectMarker::OnDrop");
		EditorEvents.DropInvoke(this, m_EditorObject);
		return true;
	}
	
	
	void Show(bool show)
	{
		override_show = show;
		m_Root.Show(show);
	}
}




class UILinkedObject : ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected EditorObject m_EditorObject = null;
	
	void ~UILinkedObject() 
	{ 
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		delete m_Root; 
	}
	
	void Update() {}
	
	void SetObject(notnull EditorObject target) 
	{
		m_EditorObject = target;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);
	}
	
	
	Widget GetLayoutRoot() { return m_Root;	}
}


class EditorWidgetEventHandler: ScriptedWidgetEventHandler
{
	protected ref Widget m_Root;
	
	void EditorWidgetEventHandler()
	{
		
	}
	
	void ~EditorWidgetEventHandler()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		delete m_Root;
	}
	
	
	
	void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);		
	}
	
	
	override bool OnSelect(Widget w, int x, int y)
	{
		Print("EditorWidgetEventHandler::OnSelect");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		return super.OnSelect(w, x, y);
	}
	
	Widget GetRoot() { return m_Root; }
	void Update() { }
}




