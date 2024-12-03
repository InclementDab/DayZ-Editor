class DeleteBrush: EditorBrush
{	
	override void DuringMouseDown(vector position)
	{
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		surface_normal.Normalize();					
		
		vector ray_pos = position + surface_normal * 10;		
		RaycastRVParams raycast_params = new RaycastRVParams(ray_pos, ray_pos + -surface_normal * 10);
		raycast_params.radius = BrushRadius;
		raycast_params.flags = CollisionFlags.ALLOBJECTS;
		raycast_params.type = ObjIntersectView;
		raycast_params.groundOnly = false;
		raycast_params.sorted = false;
		array<ref RaycastRVResult> results = {};
		if (!DayZPhysics.RaycastRVProxy(raycast_params, results, { m_BrushDecal, GetEditor().GetCamera(), GetEditor().GetPlayer() }) || results.Count() == 0) {
			return;
		}
		
		EditorObjectMap editor_objects();
		array<Object> deleted_objects = {};
		foreach (RaycastRVResult result: results) {
			if (!result || !result.obj) {
				continue;
			}
			
			Object result_object = result.obj;
			if (GetDayZGame().GetSuppressedObjectManager().IsSuppressed(result_object)) {
				continue;
			}
			
			EditorObject eo = GetEditor().GetEditorObject(result_object);
			if (eo) {
				editor_objects.InsertEditorObject(eo);
			} else {
				deleted_objects.Insert(result_object);
			}
		}
		
		if (editor_objects.Count() > 0) {
			GetEditor().DeleteObjects(editor_objects);
		}
		
		if (deleted_objects.Count() > 0) {
			GetEditor().HideMapObjects(deleted_objects);
		}
	}
}