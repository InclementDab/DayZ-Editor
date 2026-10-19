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

class EditorDragSurfaceState: Managed
{
	vector m_PositionOffset;
	float m_Heading;
	float m_Scale;
	float m_YDistance;
	ref EditorSurfaceProjectionCache m_SurfaceCache = new EditorSurfaceProjectionCache();

	void EditorDragSurfaceState(vector position_offset, float heading, float scale, float y_distance)
	{
		m_PositionOffset = position_offset;
		m_Heading = heading;
		m_Scale = scale;
		m_YDistance = y_distance;
	}
}

class EditorObjectDragHandler: EditorDragHandler
{
	protected float m_InitialScale = 1.0;
	protected float m_InitialHeading;
	protected float m_Heading;
	protected bool m_HasIndependentSurfacePlacement;
	protected ref array<Object> m_IgnoredObjects = {};
	protected ref map<EditorObject, ref EditorDragSurfaceState> m_SurfaceStates = new map<EditorObject, ref EditorDragSurfaceState>();

	override void OnDragStart(notnull EditorObject target, array<EditorObject> additional_targets = null)
	{
		super.OnDragStart(target, additional_targets);

		m_InitialScale = target.GetScale();
		m_Heading = Math.NormalizeAngle(target.GetOrientation()[0]);
		m_InitialHeading = m_Heading;

		Object player = GetEditor().GetPlayer();
		if (player) {
			m_IgnoredObjects.Insert(player);
		}
		m_IgnoredObjects.Insert(target.GetWorldObject());

		vector heading_matrix[3];
		Math3D.YawPitchRollMatrix(Vector(m_InitialHeading, 0, 0), heading_matrix);
		vector target_position = target.GetPosition();
		foreach (EditorObject additional_target: m_AdditionalDragTargets) {
			if (additional_target) {
				CacheSurfaceState(additional_target, target_position, heading_matrix);
			}
		}

		if (GetGame().IsMultiplayer()) {
			GetEditor().GetNetActionManager().SendDragSessionStart(target, m_AdditionalDragTargets);
		}
	}

	protected void CacheSurfaceState(notnull EditorObject editor_object, vector target_position, vector heading_matrix[3])
	{
		vector position_offset = editor_object.GetPosition() - target_position;
		position_offset = position_offset.InvMultiply3(heading_matrix);
		m_SurfaceStates[editor_object] = new EditorDragSurfaceState(position_offset, Math.NormalizeAngle(editor_object.GetOrientation()[0]), editor_object.GetScale(), editor_object.GetYDistance());

		Object world_object = editor_object.GetWorldObject();
		if (world_object) {
			m_IgnoredObjects.Insert(world_object);
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
		
		vector camera_transform[4];
		GetEditor().GetCamera().GetTransform(camera_transform);
		
		vector transform[4];
		target.GetBottomTransform(transform);

		vector scale_matrix[3];
		Math3D.ScaleMatrix(m_InitialScale, scale_matrix);

		Ray cursor_ray = GetEditor().GetCursorRayModeSafe();
		bool ground_mode = GetEditor().GroundMode;
		Raycast cursor_raycast = GetEditor().GetCursorRaycastModeSafeEx(m_IgnoredObjects, ground_mode);
			
		vector cursor_pos = cursor_ray.GetPoint(10.0);
		if (cursor_raycast) {
			cursor_pos = cursor_raycast.Bounce.Position;
		}
		vector cursor_surface_position = cursor_pos;

		vector icon_position = target.GetBottomCenter();
		vector object_up_direction = target.GetWorldObject().GetDirectionUp();
		
		float distance_to_ground = 0;
		vector surface_normal;
		bool has_surface = EditorSurfacePlacement.GetSurfaceNormal(cursor_raycast, ground_mode, surface_normal);
		bool snap_to_surface = has_surface && !ground_mode;
		bool align_to_surface = has_surface && GetEditor().MagnetMode;
		bool use_origin_transform = false;

		if (ground_mode) {
			Ray downward_ray = new Ray(transform[3], -vector.Up);
			Raycast downward_raycast = downward_ray.PerformRaycastRV(target.GetWorldObject(), null, 0, 1000, ObjIntersect.View, true);
			if (downward_raycast) {
				distance_to_ground = downward_raycast.Length();
				icon_position[1] = icon_position[1] - distance_to_ground;
			}
		}
								
		// Handle Z-Only motion
		if (KeyState(KeyCode.KC_LMENU)) {			
			// This should always be ortho			
			vector forward_plane = object_up_direction * camera_transform[0];
			Plane3D z_plane = new Plane3D(forward_plane, transform[3]);
			vector intersect = z_plane.Intersect(cursor_ray);
			
			vector up_dir_matrix[4];
			Math3D.DirectionAndUpMatrix(forward_plane, object_up_direction, up_dir_matrix);
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
					m_Heading = Math.NormalizeAngle(cursor_intersect_dir.VectorToAngles()[0]);

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
			if (!align_to_surface || !EditorSurfacePlacement.GetMagnetTransform(m_Heading, surface_normal, m_InitialScale, cursor_pos, transform_new)) {
				target.GetTransform(transform_new);
			}

			if (snap_to_surface) {
				cursor_pos[1] = EditorSurfacePlacement.GetSnappedHeight(cursor_pos[1], surface_normal, target.GetYDistance(), m_InitialScale);
				use_origin_transform = true;
			} else if (ground_mode) {
				cursor_pos[1] = cursor_pos[1] + distance_to_ground;
			}

			transform_new[3] = cursor_pos;
			copyarray(transform, transform_new);
		}

		if (use_origin_transform) {
			target.SetTransform(transform);
		} else {
			target.SetBottomTransform(transform);
		}
		target.Update();

		// Use the object's actual origin transform to preserve every additional object's relative offset.
		vector transform_from_object_center[4];
		target.GetTransform(transform_from_object_center);
		Math3D.MatrixOrthogonalize4(transform_from_object_center);

		vector group_heading_matrix[3];
		float heading_delta;
		vector target_position = target.GetPosition();
		if (align_to_surface && use_origin_transform && additional_drag_targets.Count() > 0) {
			Math3D.YawPitchRollMatrix(Vector(m_Heading, 0, 0), group_heading_matrix);
			heading_delta = Math.NormalizeAngle(m_Heading - m_InitialHeading);
		}
				
		// Handle all child objects
		foreach (EditorObject selected_object: additional_drag_targets) {
			if (selected_object == target) {
				continue;
			}

			EditorDragSurfaceState surface_state = m_SurfaceStates[selected_object];
			if (align_to_surface && use_origin_transform && surface_state) {
				// Keep group layout heading-only; each object supplies its own surface tilt.
				vector projected_position = surface_state.m_PositionOffset.Multiply3(group_heading_matrix) + target_position;
				vector projection_start = EditorSurfacePlacement.GetSurfaceProjectionStart(projected_position, cursor_surface_position, surface_normal, surface_state.m_YDistance * surface_state.m_Scale);
				vector selected_surface_position;
				vector selected_surface_normal;
				if (EditorSurfacePlacement.GetSurfaceBelowCached(projection_start, m_IgnoredObjects, false, surface_state.m_SurfaceCache, selected_surface_position, selected_surface_normal)) {
					vector selected_transform[4];
					float selected_heading = Math.NormalizeAngle(surface_state.m_Heading + heading_delta);
					if (EditorSurfacePlacement.GetMagnetTransform(selected_heading, selected_surface_normal, surface_state.m_Scale, projected_position, selected_transform)) {
						if (snap_to_surface) {
							selected_transform[3][1] = EditorSurfacePlacement.GetSnappedHeight(selected_surface_position[1], selected_surface_normal, surface_state.m_YDistance, surface_state.m_Scale);
						}

						selected_object.SetTransform(selected_transform);
						m_HasIndependentSurfacePlacement = true;
						continue;
					}
				}
			}

			array<vector> local_transform_array = m_LocalTransformsToTarget[selected_object];
			vector local_additional_mat[4] = {
				local_transform_array[0],
				local_transform_array[1],
				local_transform_array[2],
				local_transform_array[3]
			};
			
			vector output_additional_mat[4];
			Math3D.MatrixMultiply4(transform_from_object_center, local_additional_mat, output_additional_mat);
			selected_object.SetTransform(output_additional_mat);
		}

		if (GetGame().IsMultiplayer()) {
			int packed_data[4];
			EditorNetUtils.PackTransform(target.GetPosition(), target.GetOrientation(), target.GetScale(), packed_data);
			GetEditor().GetNetActionManager().SendDragSessionUpdate(target.Uuid, packed_data);
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
		if (GetGame().IsMultiplayer() && m_Target) {
			int packed_data[4];
			EditorNetUtils.PackTransform(m_Target.GetPosition(), m_Target.GetOrientation(), m_Target.GetScale(), packed_data);
			GetEditor().GetNetActionManager().SendDragSessionEnd(m_Target.Uuid, packed_data);
		}

		// Manually replicate the cleanup logic from EditorDragHandler.
		if (m_RewindAction) {
			// Finalize undo/redo action with the 'after' state.
			array<EditorObject> all_dragged_objects = { m_Target };
			if (m_AdditionalDragTargets) {
				all_dragged_objects.InsertAll(m_AdditionalDragTargets);
			}

			foreach (EditorObject dragged_object: all_dragged_objects) {
				if (dragged_object) {
					dragged_object.Update();
					m_RewindAction.InsertRedoParameter(dragged_object.GetTransformArray());
				}
			}

			GetEditor().InsertAction(m_RewindAction);
		}

		// Keep high-frequency drag updates parent-only, then reliably publish exact child transforms once.
		if (m_HasIndependentSurfacePlacement && GetGame().IsMultiplayer() && m_AdditionalDragTargets && m_AdditionalDragTargets.Count() > 0) {
			GetEditor().GetNetActionManager().SendTransformUpdate(m_AdditionalDragTargets);
		}

		// This manually performs the cleanup from the base class's OnDragFinish,
		// because we are intentionally not calling super.OnDragFinish() to prevent old RPCs.
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(_OnDragging);

		if (m_Target) {
			m_Target.IsBeingDragged = false;
		}

		if (m_AdditionalDragTargets) {
			foreach (EditorObject child_object: m_AdditionalDragTargets) {
				if (child_object) {
					child_object.IsBeingDragged = false;
				}
			}
		}

		m_IsDragging = false;
		m_Target = null;
		m_AdditionalDragTargets = null;
		m_LocalTransformsToTarget = null;
		m_RewindAction = null;
		m_IgnoredObjects = null;
		m_SurfaceStates = null;
	}
}
