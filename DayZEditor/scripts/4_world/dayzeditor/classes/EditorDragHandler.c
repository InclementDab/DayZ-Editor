
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
			int x, y;
			GetCursorPos(x, y);
			OnDragging(x, y);
		} else {
			OnDragFinish();
		}
	}
	
	void OnDragging(int x, int y) { }
}


class ObjectDragHandler: DragHandler
{
	override void OnDragging(int x, int y)
	{
		vector begin_pos = GetGame().GetCurrentCameraPosition();
		vector end_pos = begin_pos + GetGame().GetPointerDirection() * 3000;
		vector contact_pos, contact_dir;
		int component;
		
		DayZPhysics.RaycastRV(begin_pos, end_pos, contact_pos, contact_dir, component, null, null, null, false, true);
		m_EditorObject.SetPosition(contact_pos);
		
	}
}


class MapDragHandler: DragHandler
{
	
	override void OnDragging(int x, int y)
	{
		
		
		
	}
}


