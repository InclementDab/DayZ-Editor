
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
		vector cursor_pos, contact_dir;
		int component;
		
		DayZPhysics.RaycastRV(begin_pos, end_pos, cursor_pos, contact_dir, component, null, null, null, false, true);
		
				
		vector size = m_EditorObject.GetSize();
		vector ground_position = GetGroundPosition(transform);
		vector surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		
		if (KeyState(KeyCode.KC_LMENU)) {
			
			cursor_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), ground_position);
			cursor_pos[1] = cursor_pos[1] + size[1]/2;
			transform[3] = ground_position + transform[1] * vector.Distance(ground_position, cursor_pos);
		}
		
		else if (KeyState(KeyCode.KC_LSHIFT)) {
					
			transform = { "1 0 0", "0 1 0", "0 0 1", transform[3] };
			vector cursor_delta = cursor_pos - transform[3];
			float angle = Math.Atan2(cursor_delta[0], cursor_delta[2]) * Math.RAD2DEG;	
			m_EditorObject.PlaceOnSurfaceRotated(transform, ground_position, surface_normal[0] * -1, surface_normal[2] * -1, angle * -1, EditorSettings.MAGNET_PLACEMENT);			
		}
		
		else {
			
			transform[3] = cursor_pos;
			transform[3][1] = cursor_pos[1] + size[1]/2;
		}
		
		
		
	}
}


class MapDragHandler: DragHandler
{
	
	override void OnDragging(out vector transform[4])
	{
		
		
		
	}
}


