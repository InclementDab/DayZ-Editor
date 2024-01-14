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
		array<EditorObject> selected_objects = EditorObject.SelectedObjects;
		foreach (EditorObject selected_object: selected_objects) {
			drag_undo.InsertUndoParameter(selected_object.GetTransformArray());
		}
		
		vector transform[4];
		m_EditorObject.GetWorldObject().GetTransform(transform);
		// hmm
		//m_Editor.AddInHand(m_EditorObject);
		
		while (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK && !(GetMouseState(MouseState.RIGHT) & MB_PRESSED_MASK)) {
			OnDragging(transform, m_EditorObject);
			m_EditorObject.GetWorldObject().SetTransform(transform);
			Sleep(10);
		} 
		
		foreach (EditorObject selected_object_post: selected_objects) {
			drag_undo.InsertRedoParameter(selected_object_post.GetTransformArray());
		}
		
		GetEditor().InsertAction(drag_undo);
		
		// hmmm
		//m_Editor.RemoveFromHand(m_EditorObject);
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