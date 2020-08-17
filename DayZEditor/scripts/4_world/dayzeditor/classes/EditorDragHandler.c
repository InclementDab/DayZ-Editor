
class DragHandler
{
	protected ref EditorObject m_EditorObject;
	void DragHandler(EditorObject target)
	{
		m_EditorObject = target;
	}
	
	void OnDragStart()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(_OnDragging);
	}
	
	void OnDragFinish()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Remove(_OnDragging);
	}
	
	private void _OnDragging()
	{
		if (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) {
			vector transform[4];
			m_EditorObject.GetTransform(transform);
			OnDragging(transform);
			m_EditorObject.SetTransform(transform);
			m_EditorObject.Update();
		} else {
			OnDragFinish();
		}
	}
	
	void OnDragging(out vector transform[4]) { }
	
	
	vector GetGroundPosition(vector transform[4])
	{
		// Raycast ground below object
		vector ground, ground_dir; 
		int component;
		DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -1000, ground, ground_dir, component, null, null, null, false, true);
		return ground;
	}
}


class ObjectDragHandler: DragHandler
{
	override void OnDragging(out vector transform[4])
	{
		vector begin_pos = GetGame().GetCurrentCameraPosition();
		vector end_pos = begin_pos + GetGame().GetPointerDirection() * 3000;
		vector contact_pos, contact_dir;
		int component;
		
		DayZPhysics.RaycastRV(begin_pos, end_pos, contact_pos, contact_dir, component, null, null, null, false, true);
		
		vector size = m_EditorObject.GetSize();
		vector cursor_position = contact_pos;
		
		if (KeyState(KeyCode.KC_LMENU)) {
			vector ground_position = GetGroundPosition(transform);
			cursor_position = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), ground_position);
			cursor_position[1] = cursor_position[1] + size[1]/2;
			transform[3] = ground_position + transform[1] * vector.Distance(ground_position, cursor_position);
			
		}
		
		else if (KeyState(KeyCode.KC_LSHIFT)) {
			
			
		}
		
		else {
			
			transform[3] = contact_pos;
			transform[3][1] = contact_pos[1] + size[1] / 2;
		}
		
		
		
	}
}


class MapDragHandler: DragHandler
{
	
	override void OnDragging(out vector transform[4])
	{
		
		
		
	}
}


