class EditorSurfacePlacement
{
	protected static const float SURFACE_RAYCAST_DISTANCE = 1000.0;

	static bool GetSurfaceBelow(notnull EditorObject editor_object, out vector surface_position, out vector surface_normal)
	{
		vector transform[4];
		editor_object.GetTransform(transform);

		vector raycast_normal;
		int component;
		if (!DayZPhysics.RaycastRV(transform[3], transform[3] + vector.Up * -SURFACE_RAYCAST_DISTANCE, surface_position, raycast_normal, component, null, null, editor_object.GetWorldObject(), false, true)) {
			return false;
		}

		surface_normal = GetGame().SurfaceGetNormal(surface_position[0], surface_position[2]);
		if (surface_normal.LengthSq() == 0) {
			surface_normal = raycast_normal;
		}

		if (surface_normal.LengthSq() == 0) {
			return false;
		}

		surface_normal.Normalize();
		return true;
	}

	static void GetAlignedTransform(notnull EditorObject editor_object, vector surface_normal, out vector aligned_transform[4])
	{
		// Pitch and roll from terrain.
		vector object_angles = editor_object.GetOrientation();
		object_angles[1] = 0;
		object_angles[2] = 0;

		vector surface_angles = surface_normal.VectorToAngles();
		surface_angles[1] = surface_angles[1] + 270;
		surface_angles[0] = Math.Clamp(surface_angles[0], 0, 360);
		surface_angles[1] = Math.Clamp(surface_angles[1], 0, 360);
		surface_angles[2] = Math.Clamp(surface_angles[2], 0, 360);
		object_angles[0] = object_angles[0] + (360 - surface_angles[0]);

		vector object_matrix[3];
		vector surface_matrix[3];
		vector aligned_matrix[3];
		Math3D.YawPitchRollMatrix(object_angles, object_matrix);
		Math3D.YawPitchRollMatrix(surface_angles, surface_matrix);
		Math3D.MatrixMultiply3(surface_matrix, object_matrix, aligned_matrix);

		float scale = editor_object.GetScale();
		aligned_transform[0] = aligned_matrix[0] * scale;
		aligned_transform[1] = aligned_matrix[1] * scale;
		aligned_transform[2] = aligned_matrix[2] * scale;
		aligned_transform[3] = editor_object.GetPosition();
	}

	static void GetSnappedTransform(notnull EditorObject editor_object, vector surface_position, vector surface_normal, out vector snapped_transform[4])
	{
		GetAlignedTransform(editor_object, surface_normal, snapped_transform);

		// Clipping bounds can extend below the models, we use bounding-center here for height.
		float bottom_offset = editor_object.GetYDistance() * editor_object.GetScale();
		snapped_transform[3][1] = surface_position[1] + surface_normal[1] * bottom_offset;
	}
}
