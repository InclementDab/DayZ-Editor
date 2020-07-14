

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
	}
	
	
		
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarkerHandler::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		m_Root.SetAlpha(0.35);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		m_Root.SetAlpha(0.9);
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_Root.SetAlpha(0.35);
		return true;
	}
		
	override bool OnDrag(Widget w, int x, int y)
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DragUpdate);
		return true;
		
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(DragUpdate);
		//Editor.RemoveBoundingBox();
		//	Editor.ActiveBoundingBox = Editor.CreateBoundingBox(Editor.SelectedObject);
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button == 0) {
			SetFocus(m_Root);
			Editor.SelectObject(Editor.ObjectUnderCursor);
			return true;
		}
		
		if (button == 1) {			
			EditorContextMenu.ShowContextMenu(x, y);
			return true;
		}
		
		return false;
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
	
	void DragUpdate()
	{
		Object obj = Editor.SelectedObject;
		vector cursor_pos;
		vector size = GetObjectSize(obj);
		set<Object> o;
		
		Input input = GetGame().GetInput();
		
		vector pos = obj.GetPosition();
		
		// Handle Z only motion
		if (input.LocalValue("UALookAround")) {	
			float dist = vector.Distance(GetGame().GetCurrentCameraPosition(), pos);
			cursor_pos = MousePosToRay(o, obj, dist);			
			vector v3 = {pos[0], cursor_pos[1] + size[1]/2, pos[2]};
			obj.SetPosition(v3);
			
		// Handle XY Plane Rotation
		} else if (input.LocalValue("UATurbo")) {
			cursor_pos = MousePosToRay(o, obj);
			vector direction = vector.Direction(obj.GetPosition(), cursor_pos);
			direction[1] = 0;
			obj.SetDirection(direction);
		
		} else {
			cursor_pos = MousePosToRay(o, Editor.SelectedObject);
			cursor_pos[1] = cursor_pos[1] + size[1]/2;
			Editor.SelectedObject.SetPosition(cursor_pos);
		}
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
	protected TextWidget m_EditorListItemText;	
	
	void ~EditorObject()
	{
		Print("~EditorObject");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		delete m_Root; delete m_EditorObjectMarkerPanel; delete m_EditorObjectMarkerImage;
		delete m_EditorListItemFrame; delete m_EditorListItemText;
	}

	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObject::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		
		m_EditorObjectMarkerPanel = m_Root.FindAnyWidget("EditorObjectMarkerPanel");
		m_EditorObjectMarkerImage = ImageWidget.Cast(m_Root.FindAnyWidget("EditorObjectMarkerImage"));		
		
		m_EditorListItemFrame = m_Root.FindAnyWidget("EditorListItemFrame");
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
			
			//if (w == GetFocus()) return true;
			switch (w) {
				case (m_EditorListItemFrame):
					if (Editor.IsPlacing())
						delete Editor.ObjectInHand;
					SetFocus(m_Root);
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
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorObject::OnFocusLost");
		m_EditorListItemFrame.SetColor(0xFFFFFFFF);
		return true;
	}	
	
	Object GetWorldObject()
	{
		return m_Object;
	}
	
	Widget GetLayoutRoot()
	{
		return m_EditorListItemFrame;
	}
}

