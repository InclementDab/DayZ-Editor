enum EDragFlags
{
	FREE = 0,

	AXIS_LOCK_X = 1,
	AXIS_LOCK_Y = 2,
	AXIS_LOCK_Z = 4,

	ROTATION_LOCK_X = 8,
	ROTATION_LOCK_Y = 16,
	ROTATION_LOCK_Z = 32
};

class Plane3D: Managed
{
	vector Normal, Center;

	void Plane3D(vector normal, vector center)
	{
		Normal = normal.Normalized();
		Center = center;
	}

	vector Intersect(notnull Ray source)
	{
		float denom = vector.Dot(Normal, source.Direction);
		if (Math.AbsFloat(denom) > Math.EPSILON) {
			float t = vector.Dot((Center - source.Position), Normal) / denom;
			if (t >= 0) {
				return source.Position + source.Direction * t;
			}
		}

		return vector.Zero;
	}
	
	void Debug(float size = 2, int color = LinearColor.BLUE, ShapeFlags flags = ShapeFlags.ONCE)
	{
		vector aside = vector.Aside;
		if (Math.AbsFloat(vector.Dot(Normal, aside)) == 1) {
			aside = vector.Up;
		}

		vector mat[4] = {
			aside,
			Normal,
			aside * Normal,
			Center
		};

		Math3D.MatrixOrthogonalize4(mat);

		Shape s = Shape.Create(ShapeType.BBOX, color, flags, Vector(-size, 0, -size), Vector(size, 0, size));
		s.SetMatrix(mat);

		Shape.CreateArrow(Center, Center + Normal * size, size / 2, LinearColor.WHITE, flags);
	}
}

class EditorObjectDragHandler: EditorDragHandler
{
	protected float m_InitialScale = 1.0;

	override void OnDragStart(notnull EditorObject target, array<EditorObject> additional_targets = null)
	{
		super.OnDragStart(target, additional_targets);

        if (target) {
            m_InitialScale = target.GetScale();
        }

		if (GetGame().IsMultiplayer())
		{
			GetEditor().GetNetActionManager().SendDragSessionStart(target, additional_targets);
		}
	}

	protected EDragFlags GetDragFlags()
	{
		EDragFlags flags;

		// todo, stupid
		if (KeyState(KeyCode.KC_LMENU)) {
			flags |= EDragFlags.AXIS_LOCK_Y;
		}

		return flags;
	}

	// we will accept vector4, but the transposition operation will only take place on the first 
	protected void MatrixTranspose3(vector src[3], out vector dst[3])
	{
		const int N = 3;
		const int M = 3;
		for (int n = 0; n < N * M; n++) {
			int i = n / N;
			int j = n % N;
			dst[n] = src[M * j + i];
		}
	}

	protected void MatrixTranspose4(vector src[4], out vector dst[4])
	{
		const int N = 3;
		const int M = 3;
		for (int n = 0; n < N * M; n++) {
			int i = n / N;
			int j = n % N;
			dst[n] = src[M * j + i];
		}
	}
	
	protected override void OnDragging(notnull EditorObject target, notnull array<EditorObject> additional_drag_targets)
	{
		//ScopedFunctionTimer Scope0("EditorObjectDragHandler.OnDragging");

		array<EditorObject> all_objects = {};
		all_objects.Insert(target);
		all_objects.InsertAll(additional_drag_targets);
						
		array<Object> all_object_instances = {};
		foreach (EditorObject editor_object_get_object: all_objects) {
			all_object_instances.Insert(editor_object_get_object.GetWorldObject());
		}
		
		vector camera_transform[4];
		GetEditor().GetCamera().GetTransform(camera_transform);
		
		vector transform[4];
		target.GetBottomTransform(transform);

		vector transform_without_scale[4];
		copyarray(transform_without_scale, transform);
		Math3D.MatrixOrthogonalize4(transform_without_scale);
		
		vector scale_matrix[3];
		Math3D.ScaleMatrix(m_InitialScale, scale_matrix);

		array<Object> ignored_objects = {};
		ignored_objects.InsertAll(all_object_instances);
		ignored_objects.Insert(GetEditor().GetPlayer());

		Ray cursor_ray = GetEditor().GetCursorRay();				
		int interaction_layers = -1;
		if (GetEditor().GroundMode) {
			interaction_layers &= PhxInteractionLayers.TERRAIN;
		}
		
		//Raycast cursor_raycast = cursor_ray.PerformRaycastMulti(ignored_objects, GetEditor().GetCamera().GetSettings().ViewDistance / 2, interaction_layers);
		Raycast cursor_raycast = GetEditor().GetCursorRaycastModeSafeEx(ignored_objects, GetEditor().GroundMode);
			
		vector cursor_pos = cursor_ray.GetPoint(10.0);
		if (cursor_raycast) {
			cursor_pos = cursor_raycast.Bounce.Position;
		}

		vector icon_position = target.GetBottomCenter();
		
		vector rotation_source_pos = icon_position;
		if (GetEditor().GroundMode) {
			rotation_source_pos = ProjectToGround(transform);
		}

		vector up_dir = vector.Up;		
		float distance_to_ground = 0;
		if (GetEditor().MagnetMode) {
			up_dir = cursor_raycast.Bounce.Direction;
		}

		up_dir.Normalize();
		Ray downward_ray = new Ray(transform[3], -up_dir);
		if (GetEditor().GroundMode && cursor_raycast) {			
			Raycast downward_raycast = downward_ray.PerformRaycastRV(target.GetWorldObject(), null, 0, 1000, ObjIntersect.View, true);
			if (downward_raycast) {
				distance_to_ground = downward_raycast.Length();
			}
		}
								
		// Handle Z-Only motion
		if (KeyState(KeyCode.KC_LMENU)) {			
			// This should always be ortho			
			vector forward_plane = up_dir * camera_transform[0];
			Plane3D z_plane = new Plane3D(forward_plane, transform[3]);
			vector intersect = z_plane.Intersect(cursor_ray);
			
			vector up_dir_matrix[4];
			Math3D.DirectionAndUpMatrix(forward_plane, up_dir, up_dir_matrix);
			Math3D.MatrixOrthogonalize4(up_dir_matrix);
			up_dir_matrix[3] = transform[3];
			vector local_intersect = intersect.InvMultiply4(up_dir_matrix);
			local_intersect[0] = 0;
			local_intersect[2] = 0;
			local_intersect[1] = local_intersect[1];
			intersect = local_intersect.Multiply4(up_dir_matrix);
			transform[3] = intersect;
		}
		
		// Handle XY Rotation
		else if (GetEditor().IsShiftDown()) {
			Plane3D xy_plane = new Plane3D(transform[1], icon_position);
			vector xy_intersect = xy_plane.Intersect(cursor_ray);
			if (vector.Distance(icon_position, xy_intersect) > 0.001) {
				vector cursor_intersect_dir = vector.Direction(icon_position, xy_intersect);
#ifdef DIAG_DEVELOPER
				Debug.DrawArrow(icon_position, icon_position + cursor_intersect_dir * 10, 1, LinearColor.BLUE, ShapeFlags.ONCE);
#endif

				vector cursor_dir_mat[4];
				cursor_intersect_dir.Normalize();
				if (cursor_intersect_dir.Length() > 0 && Math.AbsFloat(vector.Dot(cursor_intersect_dir, transform[1])) != 1) {
					Math3D.DirectionAndUpMatrix(cursor_intersect_dir, transform[1], cursor_dir_mat);
					Math3D.MatrixOrthogonalize3(cursor_dir_mat);

					//cursor_dir_mat[3] = icon_position.InvMultiply4(transform) + transform[3];
					Math3D.MatrixMultiply3(scale_matrix, cursor_dir_mat, cursor_dir_mat);
					cursor_dir_mat[3] = transform[3];
					transform = cursor_dir_mat;
				}
			}
		}
		
		// Plane placement
		else if (GetEditor().IsCtrlDown()) {
			Plane3D camera_plane = new Plane3D(camera_transform[2], transform[3]);
			
#ifdef DIAG_DEVELOPER
			camera_plane.Debug();
#endif
			vector camera_plane_intersect = camera_plane.Intersect(cursor_ray);			
			transform[3] = camera_plane_intersect;
		}
		// Handle regular motion
		else {
			vector transform_new[4];
			if (GetEditor().MagnetMode) {
				vector aside_new = transform[0] * up_dir;			
				Math3D.DirectionAndUpMatrix(aside_new, up_dir, transform_new);
				Math3D.MatrixMultiply3(scale_matrix, transform_new, transform_new);
			} else {
				target.GetTransform(transform_new);
			}
			
			if (GetEditor().GroundMode) {
				cursor_pos = cursor_pos + up_dir * distance_to_ground;
			}

			transform_new[3] = cursor_pos;
			copyarray(transform, transform_new);
		}

		copyarray(transform_without_scale, transform);
		Math3D.MatrixOrthogonalize4(transform_without_scale);
		
		vector transform_from_object_center[4];
		copyarray(transform_from_object_center, transform);
		transform_from_object_center[3] = Vector(0, target.GetWorldObject().GetBoundingCenter()[1], 0).Multiply4(transform_from_object_center);
		Math3D.MatrixOrthogonalize4(transform_from_object_center);
				
		// Handle all child objects
		foreach (EditorObject selected_object: additional_drag_targets) {
			if (selected_object == target) {
				continue;
			}

			array<vector> dyn_vec_arry = m_LocalTransformsToTarget[selected_object];
			vector local_additional_mat[4] = {
				dyn_vec_arry[0],
				dyn_vec_arry[1],
				dyn_vec_arry[2],
				dyn_vec_arry[3]
			};
			
			vector output_additional_mat[4];
			vector ortho_parent_mat[4];
			copyarray(ortho_parent_mat, transform_from_object_center);

			// Orthogonalize the temporary matrix, removing all scale information.
			Math3D.MatrixOrthogonalize4(ortho_parent_mat);
			Math3D.MatrixMultiply4(ortho_parent_mat, local_additional_mat, output_additional_mat);
			selected_object.SetTransform(output_additional_mat);
		}
		
		target.SetBottomTransform(transform);
		target.Update();

		if (GetGame().IsMultiplayer())
		{
			int packedData[4];
			EditorNetUtils.PackTransform(target.GetPosition(), target.GetOrientation(), target.GetScale(), packedData);
			GetEditor().GetNetActionManager().SendDragSessionUpdate(target.Uuid, packedData);
		}
	}
	
	static vector GetAveragePosition(EditorObjectMap objects)
	{
		vector avg_position;
		foreach (int id, EditorObject copy_object: objects) {
			avg_position += copy_object.GetPosition();
		}
		
		for (int i = 0; i < 3; i++) {
			avg_position[i] = avg_position[i] / objects.Count();
		}
		
		avg_position[1] = GetGame().SurfaceY(avg_position[0], avg_position[2]);
		
		return avg_position;
	}

	override void OnDragFinish()
	{
		// First, send the new reliable END RPC for the session.
		if (GetGame().IsMultiplayer() && m_Target)
		{
			int packedData[4];
			EditorNetUtils.PackTransform(m_Target.GetPosition(), m_Target.GetOrientation(), m_Target.GetScale(), packedData);
			GetEditor().GetNetActionManager().SendDragSessionEnd(m_Target.Uuid, packedData);
		}

		// Manually replicate the cleanup logic from EditorDragHandler 
		if (m_RewindAction)
		{
			// Finalize undo/redo action with the 'after' state.
			array<EditorObject> all_dragged_objects = { m_Target };
			if (m_AdditionalDragTargets)
				all_dragged_objects.InsertAll(m_AdditionalDragTargets);

			foreach(EditorObject dragged_obj : all_dragged_objects)
			{
				if (dragged_obj)
					m_RewindAction.InsertRedoParameter(dragged_obj.GetTransformArray());
			}

			GetEditor().InsertAction(m_RewindAction);
		}

		// This manually performs the cleanup from the base class's OnDragFinish,
		// because we are intentionally not calling super.OnDragFinish() to prevent old RPCs.
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(_OnDragging);

		if (m_Target)
			m_Target.IsBeingDragged = false;

		if (m_AdditionalDragTargets)
		{
			foreach(EditorObject child_obj : m_AdditionalDragTargets)
			{
				if (child_obj)
					child_obj.IsBeingDragged = false;
			}
		}

		m_IsDragging = false;
		m_Target = null;
		m_AdditionalDragTargets = null;
		m_LocalTransformsToTarget = null;
		m_RewindAction = null;
	}
}