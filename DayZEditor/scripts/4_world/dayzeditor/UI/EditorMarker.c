

static const float ALPHA_ON_SHOW = 1;
static const float ALPHA_ON_HIDE = 0.25;

class EditorMapMarker: UILinkedObject
{	
	private MapWidget m_MapWidget;
	MapWidget GetMapWidget() { return m_MapWidget; }
	
	void EditorMapMarker()
	{
		Print("EditorMapMarker");
		
	}
	
	void ~EditorMapMarker()
	{
		Print("~EditorMapMarker");
	}
	
	
	override void Update()
	{
		m_MapWidget = MapWidget.Cast(m_Root.GetParent());
		vector pos = m_MapWidget.MapToScreen(m_EditorObject.GetPosition());
		
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
		if (GetEditor().IsPlacing()) return false;
		
		// allows multiple objects to be dragged
		if (m_EditorObject.IsSelected()) 
			return true;
		
		// We want to Toggle selection if you are holding control
		if (input.LocalValue("UARunWalkTemp"))
			GetEditor().GetObjectManager().ToggleSelection(m_EditorObject);
		else {
			if (!input.LocalValue("UATurbo"))
				EditorEvents.ClearSelection(this);
			
			EditorEvents.SelectObject(this, m_EditorObject);
		}
		 // Blocks map from creating selection box
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorMapMarker::OnDrag");
		if (GetEditor().IsPlacing()) return false;
		EditorEvents.DragInvoke(this, m_EditorObject);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		Print("EditorMapMarker::OnDrop");	
		EditorEvents.DropInvoke(this, m_EditorObject);
		return true;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorMapMarker::OnFocus");
		EditorEvents.SelectObject(this, m_EditorObject);
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorMapMarker::OnFocusLost");
		EditorEvents.DeselectObject(this, m_EditorObject);
		return true;
	}
}

class EditorObjectMarker: UILinkedObject
{

	protected bool override_show = true;
	private bool MouseInside = false;
	
	void ~EditorObjectMarker()
	{
		Print("~EditorObjectMarker");
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
	
		
		vector screenpos = GetGame().GetScreenPos(position);

		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		m_Root.SetPos(screenpos[0] - 15, screenpos[1] - 15);
		
		if (override_show) {
			m_Root.Show(screenpos[2] > 0 && screenpos[2] < 1000);			
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
		if (GetEditor().IsPlacing()) return false;
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
		
		if (button == 0) {
			
			if (GetEditor().IsPlacing()) return false;
			
			// required for multiple objects to be dragged
			if (m_EditorObject.IsSelected()) 
				return true;
			
			// We want to Toggle selection if you are holding control
			if (input.LocalValue("UARunWalkTemp"))
				GetEditor().GetObjectManager().ToggleSelection(m_EditorObject);
			else {
				if (!input.LocalValue("UATurbo"))
					EditorEvents.ClearSelection(this);
				
				EditorEvents.SelectObject(this, m_EditorObject);
			}
			
			
		} else if (button == 1) {
			
			/*
			m_EditorObjectContextMenu = new UILinkedObject();
			m_EditorObjectContextWidget = g_Game.GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorContextMenu.layout");
			m_EditorObjectContextWidget.GetScript(m_EditorObjectContextMenu);
			m_EditorObjectContextMenu.SetObject(this);
			m_EditorObjectContextWidget.Show(false);
			*/
		} else return false;


		return true;
	}

	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorObjectMarker::OnDrag");
		
		if (GetEditor().IsPlacing()) return false;
		
		EditorEvents.DragInvoke(this, m_EditorObject);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		Print("EditorObjectMarker::OnDrop");
		EditorEvents.DropInvoke(this, m_EditorObject);
		return true;
	}
	
	
	// todo Editor UI Manager that manages modal windows correctly :)
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
	
	EditorObject GetEditorObject() { return m_EditorObject; }
	
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
		//GetEditor().GetUIManager().GetUpdateInvoker().Insert(Update);
	}
	
	void ~EditorWidgetEventHandler()
	{
		//GetEditor().GetUIManager().GetUpdateInvoker().Remove(Update);
		m_Root.Show(false);
		delete m_Root;
	}
	
	
	
	void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);
	}
	
	
	Widget GetRoot() { return m_Root; }
	void Update(float timeslice) { }
}




