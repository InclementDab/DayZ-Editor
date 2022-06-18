class EditorDragHandler
{
	protected Editor m_Editor;
	protected EditorObject m_EditorObject;
	void EditorDragHandler(EditorObject target)
	{
		m_EditorObject = target;
		m_Editor = GetEditor();
	}
	
	void OnDragStart()
	{
		thread _OnDragging();
	}
	
	void OnDragFinish();
	
	private void _OnDragging()
	{
		EditorAction drag_undo = new EditorAction("SetTransform", "SetTransform");
		EditorObjectMap selected_objects = GetEditor().GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {
			drag_undo.InsertUndoParameter(selected_object.GetTransformArray());
			
			if (EntityAI.Cast(selected_object.GetWorldObject())) {
				EntityAI.Cast(selected_object.GetWorldObject()).DisableSimulation(true);
			}
		}
		
		vector transform[4];
		m_EditorObject.GetTransform(transform);
		m_Editor.AddPlacingObject(m_EditorObject);
		
		while (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK && !(GetMouseState(MouseState.RIGHT) & MB_PRESSED_MASK)) {
			OnDragging(transform, m_EditorObject);
			m_EditorObject.SetTransform(transform);
			Sleep(10);
		} 
		
		foreach (EditorObject selected_object_post: selected_objects) {
			drag_undo.InsertRedoParameter(selected_object_post.GetTransformArray());
			
			if (EntityAI.Cast(selected_object_post.GetWorldObject())) {
				EntityAI.Cast(selected_object_post.GetWorldObject()).DisableSimulation(!selected_object_post.Simulate);
			}
		}
		
		GetEditor().InsertAction(drag_undo);
		
		m_Editor.RemovePlacingObject(m_EditorObject);
		OnDragFinish();
	}
	
	void OnDragging(out vector transform[4], notnull EditorObject target);
	
	vector GetGroundPosition(vector transform[4])
	{
		// Raycast ground below object
		vector ground, ground_dir; 
		int component;
		DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -1000, ground, ground_dir, component, null, null, null, false, true);
		return ground;
	}
}