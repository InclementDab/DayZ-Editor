class EditorSurfaceProjectionCache: Managed
{
	bool m_Initialized;
	bool m_GroundOnly;
	bool m_HasSurface;
	vector m_Position;
	vector m_SurfacePosition;
	vector m_SurfaceNormal;
	ref RaycastRVParams m_RaycastParams;
	ref array<ref RaycastRVResult> m_RaycastResults = {};
}

class EditorSurfacePlacement
{
	protected static const float SURFACE_RAYCAST_DISTANCE = 1000.0;
	protected static const float SURFACE_CACHE_DISTANCE_SQ = 0.0001;
	protected static const float SURFACE_PROJECTION_MARGIN = 0.1;

	static bool GetSurfaceNormal(Raycast raycast, bool use_terrain_normal, out vector surface_normal)
	{
		if (!raycast || !raycast.Bounce) {
			surface_normal = vector.Zero;
			return false;
		}

		return ResolveSurfaceNormal(raycast.Bounce.Position, raycast.Bounce.Direction, raycast.Hit, use_terrain_normal, surface_normal);
	}

	protected static bool ResolveSurfaceNormal(vector surface_position, vector raycast_normal, Object hit_object, bool use_terrain_normal, out vector surface_normal)
	{
		surface_normal = raycast_normal;
		if (use_terrain_normal || !hit_object) {
			surface_normal = GetGame().SurfaceGetNormal(surface_position[0], surface_position[2]);
			if (surface_normal.LengthSq() == 0) {
				surface_normal = raycast_normal;
			}
		}

		if (surface_normal.LengthSq() == 0) {
			return false;
		}

		surface_normal.Normalize();
		return true;
	}

	static bool GetSurfaceBelow(notnull EditorObject editor_object, out vector surface_position, out vector surface_normal)
	{
		array<Object> ignored_objects = { editor_object.GetWorldObject() };
		return GetSurfaceBelow(editor_object.GetPosition(), ignored_objects, true, surface_position, surface_normal);
	}

	static bool GetSurfaceBelow(vector position, notnull array<Object> ignored_objects, bool ground_only, out vector surface_position, out vector surface_normal)
	{
		EditorSurfaceProjectionCache surface_cache = new EditorSurfaceProjectionCache();
		return GetSurfaceBelowCached(position, ignored_objects, ground_only, surface_cache, surface_position, surface_normal);
	}

	static bool GetSurfaceBelowCached(vector position, notnull array<Object> ignored_objects, bool ground_only, notnull EditorSurfaceProjectionCache surface_cache, out vector surface_position, out vector surface_normal)
	{
		vector position_delta = position - surface_cache.m_Position;
		if (surface_cache.m_Initialized && surface_cache.m_GroundOnly == ground_only && position_delta.LengthSq() <= SURFACE_CACHE_DISTANCE_SQ) {
			if (!surface_cache.m_HasSurface) {
				return false;
			}

			surface_position = surface_cache.m_SurfacePosition;
			surface_normal = surface_cache.m_SurfaceNormal;
			return true;
		}

		surface_cache.m_Initialized = true;
		surface_cache.m_GroundOnly = ground_only;
		surface_cache.m_Position = position;
		if (!surface_cache.m_RaycastParams) {
			surface_cache.m_RaycastParams = new RaycastRVParams(position, position + vector.Up * -SURFACE_RAYCAST_DISTANCE);
			surface_cache.m_RaycastParams.radius = 0;
			surface_cache.m_RaycastParams.flags = CollisionFlags.NEARESTCONTACT;
			surface_cache.m_RaycastParams.type = ObjIntersectGeom;
			surface_cache.m_RaycastParams.sorted = true;
		} else {
			surface_cache.m_RaycastParams.begPos = position;
			surface_cache.m_RaycastParams.endPos = position + vector.Up * -SURFACE_RAYCAST_DISTANCE;
		}

		surface_cache.m_RaycastParams.groundOnly = ground_only;
		surface_cache.m_RaycastResults.Clear();
		if (!DayZPhysics.RaycastRVProxy(surface_cache.m_RaycastParams, surface_cache.m_RaycastResults, ignored_objects) || surface_cache.m_RaycastResults.Count() == 0) {
			surface_cache.m_HasSurface = false;
			return false;
		}

		RaycastRVResult surface_result = surface_cache.m_RaycastResults[0];
		if (!surface_result) {
			surface_cache.m_HasSurface = false;
			return false;
		}

		surface_cache.m_SurfacePosition = surface_result.pos;
		surface_cache.m_HasSurface = ResolveSurfaceNormal(surface_result.pos, surface_result.dir, surface_result.obj, ground_only, surface_cache.m_SurfaceNormal);
		if (!surface_cache.m_HasSurface) {
			return false;
		}

		surface_position = surface_cache.m_SurfacePosition;
		surface_normal = surface_cache.m_SurfaceNormal;
		return true;
	}

	static vector GetSurfaceProjectionStart(vector position, vector reference_surface_position, vector reference_surface_normal, float surface_offset)
	{
		if (Math.AbsFloat(reference_surface_normal[1]) <= Math.EPSILON) {
			return position;
		}

		float delta_x = position[0] - reference_surface_position[0];
		float delta_z = position[2] - reference_surface_position[2];
		float reference_height = reference_surface_position[1] - (reference_surface_normal[0] * delta_x + reference_surface_normal[2] * delta_z) / reference_surface_normal[1];
		float projection_height = reference_height + Math.Max(Math.AbsFloat(surface_offset), SURFACE_PROJECTION_MARGIN);
		position[1] = Math.Max(position[1], projection_height);
		return position;
	}

	static bool GetAlignedTransform(float heading, vector surface_normal, float scale, vector position, out vector aligned_transform[4])
	{
		if (surface_normal.LengthSq() == 0) {
			return false;
		}

		// Match vanilla hologram placement: keep heading independent from surface tilt.
		surface_normal.Normalize();
		vector object_angles = Vector(heading, 0, 0);

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

		aligned_transform[0] = aligned_matrix[0] * scale;
		aligned_transform[1] = aligned_matrix[1] * scale;
		aligned_transform[2] = aligned_matrix[2] * scale;
		aligned_transform[3] = position;
		return true;
	}

	static bool GetAlignedTransform(notnull EditorObject editor_object, vector surface_normal, out vector aligned_transform[4])
	{
		return GetAlignedTransform(editor_object.GetOrientation()[0], surface_normal, editor_object.GetScale(), editor_object.GetPosition(), aligned_transform);
	}

	static bool GetMagnetTransform(float heading, vector surface_normal, float scale, vector position, out vector magnet_transform[4])
	{
		if (surface_normal.LengthSq() == 0) {
			return false;
		}

		surface_normal.Normalize();

		vector heading_matrix[3];
		Math3D.YawPitchRollMatrix(Vector(heading, 0, 0), heading_matrix);

		// Continuous placement needs an unscaled heading basis to avoid rotation drift.
		vector direction = heading_matrix[0] * surface_normal;
		if (direction.LengthSq() <= Math.EPSILON) {
			direction = heading_matrix[2];
		}

		Math3D.DirectionAndUpMatrix(direction, surface_normal, magnet_transform);
		Math3D.MatrixOrthogonalize3(magnet_transform);
		magnet_transform[0] = magnet_transform[0] * scale;
		magnet_transform[1] = magnet_transform[1] * scale;
		magnet_transform[2] = magnet_transform[2] * scale;
		magnet_transform[3] = position;
		return true;
	}

	static float GetSnappedHeight(float surface_height, vector surface_normal, float y_distance, float scale)
	{
		return surface_height + surface_normal[1] * y_distance * scale;
	}

	static bool GetSnappedTransform(notnull EditorObject editor_object, vector surface_position, vector surface_normal, out vector snapped_transform[4])
	{
		if (!GetAlignedTransform(editor_object, surface_normal, snapped_transform)) {
			return false;
		}

		// Clipping bounds can extend below the models, we use bounding-center here for height.
		snapped_transform[3][1] = GetSnappedHeight(surface_position[1], surface_normal, editor_object.GetYDistance(), editor_object.GetScale());
		return true;
	}
}
