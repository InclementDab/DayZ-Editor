
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
		transform[3] = contact_pos;
		transform[3][1] = contact_pos[1] + size[1] / 2;
		
	}
}


class MapDragHandler: DragHandler
{
	
	override void OnDragging(out vector transform[4])
	{
		
		
		
	}
}


