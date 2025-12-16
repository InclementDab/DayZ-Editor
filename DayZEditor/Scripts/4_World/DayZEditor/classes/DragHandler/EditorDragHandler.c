class EditorDragHandler: Managed
{
	protected EditorObject m_Target;
	protected ref array<EditorObject> m_AdditionalDragTargets = {};
	protected ref EditorAction m_RewindAction;
	protected bool m_IsDragging;
	protected ref map<EditorObject, ref array<vector>> m_LocalTransformsToTarget = new map<EditorObject, ref array<vector>>();
	
	void OnDragStart(notnull EditorObject target, array<EditorObject> additional_targets = null)
	{
		m_Target = target;
		if (additional_targets) {
			m_AdditionalDragTargets = additional_targets;
		}

		m_RewindAction = new EditorAction("SetTransform", "SetTransform");
		m_RewindAction.InsertUndoParameter(m_Target.GetTransformArray());

		vector transform_without_scale[4];
		m_Target.GetTransform(transform_without_scale);
		Math3D.MatrixOrthogonalize4(transform_without_scale);
		
		foreach (EditorObject selected_object: m_AdditionalDragTargets) {
			if (selected_object != m_Target) {
				m_RewindAction.InsertUndoParameter(selected_object.GetTransformArray());
				
				vector additional_drag_target_mat[4];
				selected_object.GetWorldObject().GetTransform(additional_drag_target_mat);
				
				IEntity children = selected_object.GetWorldObject().GetChildren();
				while (children) {
					children.ClearFlags(EntityFlags.VISIBLE, false);
					children = children.GetSibling();
				}
				
				vector inv_additional_drag_target_mat[4];
				Math3D.MatrixInvMultiply4(transform_without_scale, additional_drag_target_mat, inv_additional_drag_target_mat);
				m_LocalTransformsToTarget[selected_object] = {
					inv_additional_drag_target_mat[0],
					inv_additional_drag_target_mat[1],
					inv_additional_drag_target_mat[2],
					inv_additional_drag_target_mat[3]
				};
			}
			
			selected_object.IsBeingDragged = true;
		}

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(_OnDragging);
		
		GetEditor().GetEditorHud().SetCurrentTooltip(null);
	}
	
	void OnDragFinish()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(_OnDragging);

		m_RewindAction.InsertRedoParameter(m_Target.GetTransformArray());
		foreach (EditorObject selected_object: m_AdditionalDragTargets) {
			if (selected_object != m_Target) {
				m_RewindAction.InsertRedoParameter(selected_object.GetTransformArray());
			}
			
			selected_object.Update();
			selected_object.IsBeingDragged = false;
		}
		GetEditor().InsertAction(m_RewindAction);
		
		m_Target = null;
		m_AdditionalDragTargets = {};
	}

	bool IsDragging()
	{
		return m_IsDragging;
	}
	
	protected void _OnDragging()
	{
		vector transform[4];
		m_Target.GetTransform(transform);
		
		UAInputAPI input = GetUApi();
		if (input.GetInputByID(UAFire).LocalValue()) {
			m_IsDragging = true;
			OnDragging(m_Target, m_AdditionalDragTargets);
		}

		if (input.GetInputByID(UAFire).LocalRelease()) {
			m_IsDragging = false;
			OnDragFinish();
		}
	}
	
	protected void OnDragging(notnull EditorObject target, notnull array<EditorObject> additional_drag_targets);
	
	// Raycast ground below object
	static vector ProjectToGround(vector transform[4])
	{
		vector ground, ground_dir; 
		int component;
		DayZPhysics.RaycastRV(transform[3], transform[3] + vector.Up * -1000, ground, ground_dir, component, null, null, null, false, true);
		return ground;
	}

	// Raycast ground below object, (UNTESTED)
	static bool ProjectToGround4(vector transform[4], out vector result[4])
	{		
		Ray ground_project_ray = new Ray(transform[3], -transform[1]);
		Raycast ground_project_raycast = ground_project_ray.PerformRaycastRV(null, null, 0, 1000, ObjIntersect.View, true);
		if (!ground_project_raycast) {
			return false;
		}

		vector ground_normal = GetGame().SurfaceGetNormal(ground_project_raycast.Bounce.Position[0], ground_project_raycast.Bounce.Position[2]);
		vector ground_aside = vector.Aside;
		if (Math.AbsFloat(vector.Dot(ground_normal, ground_aside)) >= 1 - Math.EPSILON) {
			ground_aside = vector.Forward;
		}
		
		result = {
			ground_aside, 
			ground_normal,
			ground_normal * ground_aside,
			ground_project_raycast.Bounce.Position
		};
							
		Math3D.MatrixOrthogonalize4(result);
	
		return true;
	}
}