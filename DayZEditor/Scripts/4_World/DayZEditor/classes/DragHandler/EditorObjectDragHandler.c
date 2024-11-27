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
}

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
		array<EditorObject> all_objects = {};
		all_objects.Insert(target);
		all_objects.InsertAll(additional_drag_targets);
		
		vector camera_transform[4];
		GetEditor().GetCamera().GetTransform(camera_transform);

		GetEditor().GetEditorHud().SetCurrentTooltip(null);

		vector transform[4];
		target.GetBottomTransform(transform);

		vector transform_without_scale[4];
		copyarray(transform_without_scale, transform);
		Math3D.MatrixOrthogonalize4(transform_without_scale);
		
		vector scale_matrix[3];
		Math3D.ScaleMatrix(transform[0].Length(), scale_matrix);

		Ray cursor_ray = GetEditor().GetCursorRay();
		Raycast cursor_raycast = GetEditor().GetCursorRaycastModeSafe(target.GetWorldObject(), GetEditor().GroundMode);
		vector cursor_pos = cursor_ray.GetPoint(10.0);
		if (cursor_raycast) {
			cursor_pos = cursor_raycast.Bounce.Position;
		}

		vector icon_position = target.GetBottomCenter();
		vector transform_ground_projection = ProjectToGround(transform);
		vector rotation_source_pos = target.GetBottomCenter();
		if (GetEditor().GroundMode) {
			rotation_source_pos = transform_ground_projection;
		}

		vector surface_normal = GetGame().SurfaceGetNormal(rotation_source_pos[0], rotation_source_pos[2]);		
		vector up_dir = transform[1];
		//if (GetEditor().GetSettings().AltMoveMode) {
		//	up_dir = transform[1];
		//}
		
		if (GetEditor().MagnetMode) {
			up_dir = surface_normal;
		}

		//up_dir.Normalize();

		if (GetEditor().GroundMode) {
			icon_position = transform[3] - transform[1].Normalized() * vector.Distance(transform_ground_projection, transform[3]);

			// always use transform[1] because GetBottomCenter is doing the opposite of this
			cursor_pos = cursor_pos + transform[1].Normalized() * vector.Distance(transform_ground_projection, transform[3]);
		}

		map<EditorObject, ref array<vector>> local_transforms_to_target = new map<EditorObject, ref array<vector>>();
		foreach (EditorObject additional_drag_target: additional_drag_targets) {
			vector additional_drag_target_mat[4];
			additional_drag_target.GetBottomTransform(additional_drag_target_mat);
			vector inv_additional_drag_target_mat[4];
			Math3D.MatrixInvMultiply4(transform_without_scale, additional_drag_target_mat, inv_additional_drag_target_mat);
			local_transforms_to_target[additional_drag_target] = {
				inv_additional_drag_target_mat[0],
				inv_additional_drag_target_mat[1],
				inv_additional_drag_target_mat[2],
				inv_additional_drag_target_mat[3]
			};
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
		else if (KeyState(KeyCode.KC_LSHIFT)) {
			Plane3D xy_plane = new Plane3D(up_dir, icon_position);
			vector xy_intersect = xy_plane.Intersect(cursor_ray);
			if (vector.Distance(icon_position, xy_intersect) > 0.001) {
				vector cursor_intersect_dir = vector.Direction(icon_position, xy_intersect);
				Debug.DrawArrow(icon_position, icon_position + cursor_intersect_dir * 10, 1, LinearColor.BLUE, ShapeFlags.ONCE);

				vector cursor_dir_mat[4];
				cursor_intersect_dir.Normalize();
				if (cursor_intersect_dir.Length() > 0 && Math.AbsFloat(vector.Dot(cursor_intersect_dir, up_dir)) != 1) {
					Math3D.DirectionAndUpMatrix(cursor_intersect_dir, up_dir, cursor_dir_mat);
					Math3D.MatrixOrthogonalize4(cursor_dir_mat);

					//cursor_dir_mat[3] = icon_position.InvMultiply4(transform) + transform[3];
					Math3D.MatrixMultiply3(scale_matrix, cursor_dir_mat, cursor_dir_mat);
					cursor_dir_mat[3] = transform[3];
					transform = cursor_dir_mat;
				}
			}
		}
		
		// Handle regular motion
		else {
			vector transform_new[4];
			if (GetEditor().MagnetMode) {
				vector aside_new = transform[0] * up_dir;			
				Math3D.DirectionAndUpMatrix(aside_new, up_dir, transform_new);
				Math3D.MatrixOrthogonalize4(transform_new);
				Math3D.MatrixMultiply3(scale_matrix, transform_new, transform_new);
			} else {
				target.GetTransform(transform_new);
			}

			transform_new[3] = cursor_pos;
			copyarray(transform, transform_new);
		}

		copyarray(transform_without_scale, transform);
		Math3D.MatrixOrthogonalize4(transform_without_scale);
				
		// Handle all child objects
		foreach (EditorObject selected_object: additional_drag_targets) {
			if (selected_object == target) {
				continue;
			}

			array<vector> dyn_vec_arry = local_transforms_to_target[selected_object];
			vector local_additional_mat[4] = {
				dyn_vec_arry[0],
				dyn_vec_arry[1],
				dyn_vec_arry[2],
				dyn_vec_arry[3]
			};
			
			Math3D.MatrixOrthogonalize4(local_additional_mat);

			vector output_additional_mat[4];
			Math3D.MatrixMultiply4(transform_without_scale, local_additional_mat, output_additional_mat);
			selected_object.SetBottomTransform(output_additional_mat);
		}
		
		target.SetBottomTransform(transform);
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