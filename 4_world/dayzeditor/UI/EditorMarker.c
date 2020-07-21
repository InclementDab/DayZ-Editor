

static const float ALPHA_ON_SHOW = 1;
static const float ALPHA_ON_HIDE = 0.4;

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
		
		m_Root.SetPos(pos[0], pos[1]);
		
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
		
			
		if (!Editor.IsPlacing()) return false;
		
		if (input.LocalValue("UATurbo"))
			m_EditorObject.Select(false);
		else if (input.LocalValue("UARunWalkTemp"))
			m_EditorObject.ToggleSelect();
		else
			m_EditorObject.Select();

		return true;
		
		

		
		return true; // Blocks map from creating selection box
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorMapMarker::OnDrag");
		if (Editor.IsPlacing()) return false;
		Editor.EditorEventHandler.DragInvoke(this, m_EditorObject);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		Print("EditorMapMarker::OnDrop");	
		Editor.EditorEventHandler.DropInvoke(this, m_EditorObject);
		return true;
	}
}




class EditorObjectMarker: UILinkedObject
{

	protected ImageWidget m_EditorObjectMarkerImage;
	
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
		vector position = m_EditorObject.GetBottomCenter();
		position += m_EditorObject.GetObject().GetPosition();
		vector screenpos = GetGame().GetScreenPos(position);
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		

		m_Root.Show(!(screenpos[0] > screen_x || screenpos[1] > screen_y || screenpos[0] <= 0 || screenpos[1] <= 0));
			
		
		
		m_Root.Show(true);
		if (m_EditorObject.IsSelected() || MouseInside) {
			m_Root.SetAlpha(ALPHA_ON_SHOW);
		} else {
			m_Root.SetAlpha(ALPHA_ON_HIDE);
		}
		
		m_Root.SetPos(screenpos[0], screenpos[1]);
		m_Root.Update();
	}

	private bool MouseInside = false;
	

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		Print("EditorMarker::OnMouseEnter");

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
		
		if (!Editor.IsPlacing()) return false;
		
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
		Editor.EditorEventHandler.DragInvoke(this, m_EditorObject);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		Print("EditorObjectMarker::OnDrop");
		Editor.EditorEventHandler.DropInvoke(this, m_EditorObject);
		return true;
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
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		m_EditorObject = target;
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
	
	override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
	{
		
		return super.OnEvent(eventType, target, parameter0, parameter1);
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




