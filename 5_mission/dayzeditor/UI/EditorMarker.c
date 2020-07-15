



class EditorObjectMarker: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	Object WorldObject = null;
	
	// Object Markers
	protected Widget m_EditorObjectMarkerPanel;
	protected ImageWidget m_EditorObjectMarkerImage;
	
	// Browser Items
	protected Widget m_EditorListItemFrame;
	protected TextWidget m_EditorListItemText;	
	
	void ~EditorObjectMarker()
	{
		Print("~EditorObjectMarker");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		delete m_Root; delete m_EditorObjectMarkerPanel; delete m_EditorObjectMarkerImage;
		delete m_EditorListItemFrame; delete m_EditorListItemText;
	}

	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarker::OnWidgetScriptInit");
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
	

	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		
		if (w == m_EditorObjectMarkerPanel)
			m_EditorObjectMarkerPanel.SetAlpha(0.75);
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == m_EditorObjectMarkerPanel)
			m_EditorObjectMarkerPanel.SetAlpha(0.25);
		return true;
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Input input = GetGame().GetInput();
		Print("EditorObject::OnMouseButtonDown");
		Print(w.GetName());
		if (button == 0) {
			
			//if (w == GetFocus()) return true;

			if (w.GetName() == "EditorListItemFrame") {

				if (Editor.IsPlacing())
					delete Editor.ObjectInHand;
				
				SetFocus(m_Root);
				
			} else if (w.GetName() == "EditorObjectMarkerPanel") {
				Print("yes");
				Editor.CreateSelection(WorldObject, !input.LocalValue("UATurbo"));
				SetFocus(m_Root);
			}			
		}
		return true;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorObject::OnFocus");
		m_EditorListItemFrame.SetColor(COLOR_BLUE);
		m_EditorObjectMarkerPanel.SetAlpha(1);
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorObject::OnFocusLost");
		m_EditorListItemFrame.SetColor(0xFFFFFFFF);
		m_EditorObjectMarkerPanel.SetAlpha(0.25);
		return true;
	}	
	

	
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorObject::OnDrag");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DragUpdate);
		return true;
		
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		Print("EditorObject::OnDrop");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(DragUpdate);
		return true;
	}
	
	
	void DragUpdate()
	{		
		vector object_position = WorldObject.GetPosition();
		vector size = GetObjectSize(WorldObject);
		set<Object> obj;
		vector cursor_projection = MousePosToRay(obj, WorldObject);
		Input input = GetGame().GetInput();
		
		
		// Handle Z only motion
		if (input.LocalValue("UALookAround")) {	
			float dist = vector.Distance(GetGame().GetCurrentCameraPosition(), object_position);
			cursor_projection = MousePosToRay(obj, WorldObject, dist);			
			foreach (Object editor_positionup_object: Editor.SelectedObjects) {
				vector v3 = { object_position[0], cursor_projection[1] + size[1]/2, object_position[2] };
				editor_positionup_object.SetPosition(v3);
				editor_positionup_object.Update();
			}
			
		// Handle XY Plane Rotation
			// needs to be updated to use transformations
		} else if (input.LocalValue("UATurbo")) {
			
			vector center_up_axis = WorldObject.GetTransformAxis(1);

			vector direction = vector.Direction(object_position, cursor_projection);
			direction[1] = 0;
			direction.Normalize();
			
			foreach (Object editor_rotate_object: Editor.SelectedObjects) {    
			    float cosAngle = direction[0];
			    float sinAngle = direction[2];
				vector loc_pos = Vector(0, 0, vector.Distance( WorldObject.GetPosition(), editor_rotate_object.GetPosition()));
			    vector rot_pos = vector.RotateAroundZero( loc_pos, center_up_axis, sinAngle, cosAngle);
			    editor_rotate_object.SetPosition(rot_pos + object_position);
			    editor_rotate_object.SetDirection(direction);
			
			    editor_rotate_object.Update();
			}
			
			
		} else {
			
			cursor_projection[1] = cursor_projection[1] + size[1]/2;			
			
			foreach (Object editor_position_object: Editor.SelectedObjects) { 
				vector pos_delta = editor_position_object.GetPosition() - object_position;
				editor_position_object.SetPosition(cursor_projection + pos_delta); 
				editor_position_object.Update();
			}
		}
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

