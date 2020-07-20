

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
		
		if (m_EditorObject.IsSelected()) 
			m_Root.SetAlpha(ALPHA_ON_SHOW);
		else m_Root.SetAlpha(ALPHA_ON_HIDE);

	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		Print("EditorMapMarker::OnMouseEnter");

		m_Root.SetAlpha(ALPHA_ON_SHOW);
		m_Root.Update();
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Print("EditorMapMarker::OnMouseLeave");

		m_Root.SetAlpha(ALPHA_ON_HIDE);
		m_Root.Update();
		return false;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorMapMarker::OnMouseButtonDown");
		
		Input input = GetGame().GetInput();
		
		if (Editor.IsPlacing()) {
			EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
			EditorObject editor_object = Editor.CreateObject(e.GetType(), e.GetWorldPosition(), vector.Up);
			editor_object.Select();
			if (!input.LocalValue("UATurbo")) delete Editor.ObjectInHand;
		} else if (input.LocalValue("UATurbo")) {
			m_EditorObject.Select(false);
		} else if (input.LocalValue("UARunWalkTemp")) {
			m_EditorObject.ToggleSelect();
		}
		
		

		
		return true; // Blocks map from creating selection box
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorMapMarker::OnDrag");
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
		if (m_EditorObject.IsSelected()) {
			m_Root.SetAlpha(ALPHA_ON_SHOW);
		} else {
			m_Root.SetAlpha(ALPHA_ON_HIDE);
		}
		
		m_Root.SetPos(screenpos[0], screenpos[1]);
		m_Root.Update();
	}


	

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		Print("EditorMarker::OnMouseEnter");

		m_Root.SetAlpha(ALPHA_ON_SHOW);
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Print("EditorMarker::OnMouseLeave");

		m_Root.SetAlpha(ALPHA_ON_HIDE);
		return true;
	}
	

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorObjectMarker::OnMouseButtonDown: " + button);
		
		return false;
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
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
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
	
	Widget GetRoot() { return m_Root; }
	void Update() { }	
}




