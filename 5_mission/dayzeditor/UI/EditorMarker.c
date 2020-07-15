

class EditorObjectMarkerHandler: ScriptedWidgetEventHandler
{
	protected ref Widget m_Root;
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
		m_Root.SetAlpha(0.9);
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_Root.SetAlpha(0.25);
		return true;
	}
		
	vector drag_start_pos;
	
	override bool OnDrag(Widget w, int x, int y)
	{
		set<Object> o;
		drag_start_pos = MousePosToRay(o);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DragUpdate);
		return true;
		
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(DragUpdate);
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button == 0) {
			Editor.OnMouseButtonPress(button)
			SetFocus(m_Root);
			
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
		m_Root.SetAlpha(0.25);
		return true;
	}
	
	
	vector target_old_position = vector.Zero;
	void DragUpdate()
	{
		Object target = Editor.ObjectUnderCursor;
		
		vector size = GetObjectSize(target);
		set<Object> obj;
		vector cursor_projection = MousePosToRay(obj, target);
		Input input = GetGame().GetInput();
		vector pos = target.GetPosition();
		
		// Handle Z only motion
		if (input.LocalValue("UALookAround")) {	
			float dist = vector.Distance(GetGame().GetCurrentCameraPosition(), pos);
			cursor_projection = MousePosToRay(obj, target, dist);			
			vector v3 = {pos[0], cursor_projection[1] + size[1]/2, pos[2]};
			
			foreach (Object editor_positionup_object: Editor.SelectedObjects) {
				editor_positionup_object.SetPosition(v3);
				editor_positionup_object.Update();
			}
			
		// Handle XY Plane Rotation
			// needs to be updated to use transformations
		} else if (input.LocalValue("UATurbo")) {
			vector direction = vector.Direction(pos, cursor_projection);
			direction[1] = 0;
			
			foreach (Object editor_rotate_object: Editor.SelectedObjects) {
				editor_rotate_object.SetDirection(direction); // i think its silly
				editor_rotate_object.Update();
			}
			
		} else {
			
			cursor_projection[1] = cursor_projection[1] + size[1]/2;
			
			vector posDelta = target.GetPosition() - target_old_position; 
			foreach ( Object editor_position_object: Editor.SelectedObjects ) { 
				Print(posDelta);
				editor_position_object.SetPosition(cursor_projection + posDelta); 
				editor_position_object.Update();
			}
			target_old_position = target.GetPosition();
		}
	}
	
	

}



class EditorObject: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	Object WorldObject = null;
	
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
	
	Widget Initialize(notnull Object obj)
	{
		Print("EditorObject::Initialize " + obj);

		WorldObject = obj;
		m_EditorListItemText.SetText(WorldObject.GetType());
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		return m_EditorListItemFrame;
	}
	
	void Update()
	{
		vector clip_info[2];
		float radius = WorldObject.ClippingInfo(clip_info);	
		vector position = AverageVectors(clip_info[0], clip_info[1]);
		position = position + WorldObject.GetPosition();
		position[1] = position[1] - clip_info[1][1];
		vector screenpos = GetGame().GetScreenPos(position);
		m_EditorObjectMarkerPanel.SetPos(screenpos[0], screenpos[1]);
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
	
	
	Widget GetLayoutRoot()
	{
		return m_EditorListItemFrame;
	}
	
	void GetMarkerPosition(out float x, out float y)
	{
		m_EditorObjectMarkerPanel.GetPos(x, y);
	}
}

