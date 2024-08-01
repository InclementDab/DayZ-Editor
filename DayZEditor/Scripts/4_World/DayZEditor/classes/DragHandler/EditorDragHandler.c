class EditorDragHandler: Managed
{
	protected EditorObject m_Target;
	protected ref array<EditorObject> m_AdditionalDragTargets = {};
	protected ref EditorAction m_RewindAction;

	void OnDragStart(notnull EditorObject target, array<EditorObject> additional_targets = null)
	{
		m_Target = target;
		if (additional_targets) {
			m_AdditionalDragTargets = additional_targets;
		}

		m_RewindAction = new EditorAction("SetTransform", "SetTransform");
		m_RewindAction.InsertUndoParameter(m_Target.GetTransformArray());

		foreach (EditorObject selected_object: m_AdditionalDragTargets) {
			m_RewindAction.InsertUndoParameter(selected_object.GetTransformArray());
		}

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(_OnDragging);
	}
	
	void OnDragFinish()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(_OnDragging);

		m_RewindAction.InsertRedoParameter(m_Target.GetTransformArray());
		foreach (EditorObject selected_object: m_AdditionalDragTargets) {
			m_RewindAction.InsertRedoParameter(selected_object.GetTransformArray());
		}
		
		GetEditor().InsertAction(m_RewindAction);
		
		m_Target = null;
		m_AdditionalDragTargets = {};
	}
	
	protected void _OnDragging()
	{
		vector transform[4];
		m_Target.GetTransform(transform);
		
		UAInputAPI input = GetUApi();
		if (input.GetInputByID(UAFire).LocalValue()) {
			OnDragging(m_Target, m_AdditionalDragTargets);
		}

		if (input.GetInputByID(UAFire).LocalRelease()) {
			OnDragFinish();
		}
	}
	
	protected void OnDragging(notnull EditorObject target, notnull array<EditorObject> additional_drag_targets);
	
	protected static vector ProjectToGround(vector transform[4])
	{
		// Raycast ground below object
		vector ground, ground_dir; 
		int component;
		DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -1000, ground, ground_dir, component, null, null, null, false, true);
		return ground;
	}
}