

static vector MousePosToRay(out set<Object> collisions, Object ignore = null, float raycast_distance = EditorSettings.OBJECT_VIEW_DISTANCE, float radius = 0)
{
	vector ray_start = GetGame().GetCurrentCameraPosition();
	vector ray_end = ray_start + GetGame().GetPointerDirection() * EditorSettings.OBJECT_VIEW_DISTANCE;
	
	vector hitPos, hitNormal;
	int hitComponentIndex;		
	collisions = new set<Object>;
	
	DayZPhysics.RaycastRV(ray_start, ray_end, hitPos, hitNormal, hitComponentIndex, collisions, NULL, ignore, false, false, 1, radius, CollisionFlags.ALLOBJECTS);
	return hitPos;
}


static vector ObjectGetSize(Object obj)
{
	vector size[2];
	vector result;
	
	float radius = obj.ClippingInfo(size);
	
	result[0] = Math.AbsFloat(size[0][0]) + Math.AbsFloat(size[1][0]);
	result[1] = Math.AbsFloat(size[0][1]) + Math.AbsFloat(size[1][1]);
	result[2] = Math.AbsFloat(size[0][2]) + Math.AbsFloat(size[1][2]);
	return result;
}