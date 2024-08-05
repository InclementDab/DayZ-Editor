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

class EditorObjectDragHandler: EditorDragHandler
{
	protected float m_LastAngle;

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
		vector target_transform[4];
		target.GetTransform(target_transform);

		vector cursor_transform[4];
		GetEditor().GetCursorTransform(cursor_transform);

		GetEditor().GetEditorHud().SetCurrentTooltip(null);

		Ray cursor_ray = GetEditor().GetCursorRay();
		bool use_building_collisions = GetEditor().CollisionMode;

		array<Object> raycast_excludes = {};
		raycast_excludes.Insert(target.GetWorldObject());

		Raycast cursor_raycast = cursor_ray.PerformRaycastRVEX(0, GetEditor().GetCamera().GetSettings().ViewDistance, ObjIntersectView, raycast_excludes, !use_building_collisions);

		vector cursor_pos = cursor_ray.GetPoint(10.0);
		if (cursor_raycast) {
			cursor_pos = cursor_raycast.Bounce.Position;
		}

		vector transform_ground_projection = ProjectToGround(target_transform);

		//vector test_transform[4];
		//Math3D.MatrixMultiply3(cursor_transform, target_transform, test_transform);

		//MatrixTranspose4(target_transform, test_transform);
		//test_transform[3] = target_transform[3];

		//cursor_transform[3] = target_transform[3];

		//Shape.CreateMatrix(test_transform);
		//Shape.CreateMatrix(cursor_transform);
		
		//target_transform[3] = cursor_pos;
		
		vector transform[4];
		target.GetTransform(transform);
		vector size, ground_position, surface_normal, local_dir, local_ori;
		vector deltapos = target.GetPosition();
		size = target.GetSize();
		float scale = target.GetScale();
		ground_position = ProjectToGround(transform);
		surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		float angle;
		int i;
		
		// Handle Z-Only motion
		// Todo will people want this as a keybind?
		if (KeyState(KeyCode.KC_LMENU)) {
			cursor_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), target.GetBottomCenter());
			cursor_pos[1] = cursor_pos[1] + size[1] / 2;
			if (GetEditor().MagnetMode) {
				transform[3] = ground_position + transform[1] * vector.Distance(ground_position, cursor_pos + GetGame().GetCurrentCameraDirection() * 1);
			} else {
				transform[3][1] = cursor_pos[1];
			}
		}
		
		// Handle XY Rotation
		else if (KeyState(KeyCode.KC_LSHIFT)) {
			vector cursor_delta = ground_position - Editor.CurrentMousePosition;
			local_ori = target.GetOrientation();
			angle = Math.Atan2(cursor_delta[0], cursor_delta[2]);
			local_ori[0] = local_ori[0] + ((angle - m_LastAngle) * Math.RAD2DEG);
			local_ori.RotationMatrixFromAngles(transform);
			
			for (i = 0; i < 3; i++) {
				transform[i] = transform[i] * scale;
			}
			
			if (GetEditor().MagnetMode) {
				local_dir = vector.Direction(ground_position, cursor_pos);
				local_dir.Normalize();
				transform[0] = surface_normal * local_dir;
				transform[1] = surface_normal;
				transform[2] = surface_normal * (local_dir * vector.Up);
				
			} else {
				if (GetEditor().GroundMode) {
					transform[3] = ground_position + transform[1] * vector.Distance(ground_position, transform[3]);
				}
			}
		}
		
		// Handle regular motion
		else {
			if (GetEditor().MagnetMode) {
				local_ori = target.GetWorldObject().GetDirection();
				transform[0] = surface_normal * local_ori;
				transform[1] = surface_normal;
				transform[2] = surface_normal * (local_ori * vector.Up);
			}
			
			if (GetEditor().GroundMode) {
				if (GetEditor().MagnetMode) {
					transform[3] = cursor_pos + surface_normal * vector.Distance(ground_position, transform[3]);				
				} else {
					transform[3] = cursor_pos + transform[1] * vector.Distance(ground_position, transform[3]);
				}
				
			} else {
				transform[3] = cursor_pos;
				transform[3][1] = transform[3][1] + target.GetYDistance();
			} 			
		}
		
		deltapos = transform[3] - deltapos;
		
		// Handle all child objects
		foreach (EditorObject selected_object: additional_drag_targets) {
			if (selected_object == target) { 
				continue; 
			}
			
			vector selected_pos = selected_object.GetPosition();
			vector selected_ori = selected_object.GetOrientation();
			
			// Handle Z-Only motion
			if (KeyState(KeyCode.KC_LMENU)) {
				selected_object.SetPosition(selected_object.GetPosition() + deltapos);
			}
			
			else if (KeyState(KeyCode.KC_LSHIFT)) {
				if (angle - m_LastAngle == 0) {
					continue;
				}
				
				selected_object.SetPosition(EditorMath.RotateAroundPoint(transform[3], selected_object.GetPosition(), vector.Up, Math.Cos(angle - m_LastAngle), Math.Sin(angle - m_LastAngle)));
				vector new_ori = selected_object.GetOrientation();
				new_ori[0] = new_ori[0] + ((angle - m_LastAngle) * Math.RAD2DEG);
				selected_object.SetOrientation(new_ori);
			} else {
				selected_object.SetPosition(selected_object.GetPosition() + deltapos);
			}
		}
		
		m_LastAngle = angle;
		
		target.SetTransform(transform);
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
}