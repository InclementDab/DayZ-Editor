static vector MousePosToRay(out set<Object> collisions, Object ignore = null, float raycast_distance = 3000/*EditorSettings.OBJECT_VIEW_DISTANCE*/, float radius = 0, bool groundonly = false)
{
	vector ray_start = GetGame().GetCurrentCameraPosition();
	vector ray_end = ray_start + GetGame().GetPointerDirection() * raycast_distance;
	
	vector hitPos, hitNormal;
	int hitComponentIndex;		
	collisions = new set<Object>;
	
	Object hit_object;
	float hit_else;
	if (!groundonly && DayZPhysics.RayCastBullet(ray_start, ray_end, PhxInteractionLayers.BUILDING, ignore, hit_object, hitPos, hitNormal, hit_else)) {
		return hitPos;
	}

	DayZPhysics.RaycastRV(ray_start, ray_end, hitPos, hitNormal, hitComponentIndex, collisions, NULL, ignore, false, groundonly, 1, radius, CollisionFlags.ALLOBJECTS);
	
	
	return hitPos;
}

static vector ObjectGetSize(notnull Object obj)
{
	vector size[2];
	vector result;
	
	obj.ClippingInfo(size);
	result[0] = Math.AbsFloat(size[0][0]) + Math.AbsFloat(size[1][0]);
	result[1] = Math.AbsFloat(size[0][1]) + Math.AbsFloat(size[1][1]);
	result[2] = Math.AbsFloat(size[0][2]) + Math.AbsFloat(size[1][2]);
	return result;
}

