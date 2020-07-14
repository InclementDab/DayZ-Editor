

class EditorObjectMarkerHandler: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	
	void EditorObjectMarkerHandler()
	{
		Print("EditorObjectMarkerHandler");
	}
		
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarkerHandler::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		Print("EditorObjectMarkerHandler::OnMouseEnter");
		
		
		return true;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		//Print("EditorObjectMarkerHandler::OnDragging");
		
		set<Object> o;
		vector v = Editor.MousePosToRay(o, Editor.CurrentSelectedObject);
		Editor.CurrentSelectedObject.SetPosition(v);
		
		return true;
	}
}



class EditorObject: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected Object m_Object = null;
	
	// Object Markers
	protected Widget m_EditorObjectMarkerPanel;
	protected ImageWidget m_EditorObjectMarkerImage;
	
	// Browser Items
	protected Widget m_EditorListItemFrame;
	protected Widget m_EditorListItemPanel;
	protected TextWidget m_EditorListItemText;	
	
	void ~EditorObject()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		Print("~EditorObject");
	}
	
		
	
	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObject::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		
		m_EditorObjectMarkerPanel = m_Root.FindAnyWidget("EditorObjectMarkerPanel");
		m_EditorObjectMarkerImage = ImageWidget.Cast(m_Root.FindAnyWidget("EditorObjectMarkerImage"));
		
		m_EditorListItemFrame = m_Root.FindAnyWidget("EditorListItemFrame");
		m_EditorListItemPanel = m_Root.FindAnyWidget("EditorListItemPanel");
		m_EditorListItemText = TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemText"));
	}
	
			
	void Update()
	{
		if (m_Object != null) {
			vector position = m_Object.GetPosition();
			vector screenpos = GetGame().GetScreenPos(position);
			m_EditorObjectMarkerPanel.SetPos(screenpos[0], screenpos[1]);
		}		
	}
	
	void Initialize(Object obj, Widget item_browser)
	{
		Print("EditorObject::Initialize " + obj);

		m_Object = obj;
		m_EditorListItemText.SetText(m_Object.GetType());
		item_browser.AddChild(m_EditorListItemFrame);
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorObject::OnMouseButtonDown");
		if (button == 0) {
			
			if (m_Root == GetFocus()) return true;
			switch (w) {
				case (m_EditorListItemPanel):
					if (Editor.IsPlacing())
						delete Editor.ObjectInHand;
					break;
				
				case (m_EditorObjectMarkerPanel):
					SetFocus(m_Root);
					break;
				
				default:
					Print("EditorObject::MouseDown::Default");
					break;
					
			}
		}
		
		
		return true;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorObject::OnFocus");
		m_EditorListItemFrame.SetColor(COLOR_BLUE);
		Editor.SetActiveObject(m_Object);
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorObject::OnFocusLost");
		m_EditorListItemFrame.SetColor(0xFFFFFFFF);
		Editor.ClearActiveObject();
		return true;
	}	
		
	
	
	Object GetWorldObject()
	{
		return m_Object;
	}
	
	Widget GetLayoutRoot()
	{
		return m_Root;
	}
}

