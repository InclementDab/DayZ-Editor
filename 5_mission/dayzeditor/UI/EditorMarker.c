

class EditorObjectMarkerHandler: ScriptedWidgetEventHandler
{
	protected ref Widget m_Root;
	
	void EditorObjectMarkerHandler()
	{
		Print("EditorObjectMarkerHandler");
	}
	
	void ~EditorObjectMarkerHandler()
	{
		Print("~EditorObjectMarkerHandler");
		//delete m_Root;
	}
		
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarkerHandler::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		m_Root.SetAlpha(0.25);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		Print("EditorObjectMarkerHandler::OnMouseEnter");
		
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		
		return true;
	}
		
	override bool OnDrag(Widget w, int x, int y)
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		return true;
		
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		return true;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		/*Input input = GetGame().GetInput();
				
		if (input.LocalPress("UATempRaiseWeapon")) {
			Print("Right Click");
			int x, y;
			GetMousePos(x, y);
			EditorContextMenu.ShowContextMenu(x, y);
		}*/
		
		Print("OnClick");
		Print(button);
		
		
		return true;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		m_Root.SetAlpha(1);
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		m_Root.SetAlpha(0.35);
		return true;
	}
	
	void Update()
	{
		Object obj = Editor.CurrentSelectedObject;
		vector cursor_pos;
		vector size = GetObjectSize(obj);
		set<Object> o;
		
		Input input = GetGame().GetInput();
		
		if (input.LocalValue("UALookAround")) {
			vector pos = obj.GetPosition();
			float dist = vector.Distance(GetGame().GetCurrentCameraPosition(), pos);
			cursor_pos = Editor.MousePosToRay(o, Editor.CurrentSelectedObject, dist);			
			vector v2 = {pos[0], cursor_pos[1] + size[1]/2, pos[2]};
			Editor.CurrentSelectedObject.SetPosition(v2);
			
			
		} else {
			cursor_pos = Editor.MousePosToRay(o, Editor.CurrentSelectedObject);
			cursor_pos[1] = cursor_pos[1] + size[1]/2;
			Editor.CurrentSelectedObject.SetPosition(cursor_pos);
		}
	}
	
	

}



class EditorObject: ScriptedWidgetEventHandler
{
	protected ref Widget m_Root;
	protected Object m_Object = null;
	
	// Object Markers
	protected ref Widget m_EditorObjectMarkerPanel;
	protected ref ImageWidget m_EditorObjectMarkerImage;
	
	// Browser Items
	protected ref Widget m_EditorListItemFrame;
	protected ref Widget m_EditorListItemPanel;
	protected TextWidget m_EditorListItemText;	
	
	void ~EditorObject()
	{
		Print("~EditorObject");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}

	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObject::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		
		m_EditorObjectMarkerPanel = m_Root.FindAnyWidget("EditorObjectMarkerPanel");
		m_EditorObjectMarkerImage = ImageWidget.Cast(m_Root.FindAnyWidget("EditorObjectMarkerImage"));
		//m_EditorObjectMarkerImage.LoadImageFile(0, "");
		
		
		m_EditorListItemFrame = m_Root.FindAnyWidget("EditorListItemFrame");
		m_EditorListItemPanel = m_Root.FindAnyWidget("EditorListItemPanel");
		m_EditorListItemText = TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemText"));
		
	}
	
			
	void Update()
	{
		if (m_Object != null) {
			vector position = m_Object.GetPosition();
			vector size = GetObjectSize(m_Object);
			position[1] = position[1] - size[1]/2;
			vector screenpos = GetGame().GetScreenPos(position);
			m_EditorObjectMarkerPanel.SetPos(screenpos[0], screenpos[1]);
		}		
	}
	
	Widget Initialize(Object obj)
	{
		Print("EditorObject::Initialize " + obj);

		m_Object = obj;
		m_EditorListItemText.SetText(m_Object.GetType());
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		return m_EditorListItemFrame;
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
		m_Root.SetColor(COLOR_BLUE);
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorObject::OnFocusLost");
		m_Root.SetColor(0xFFFFFFFF);
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

