static vector MousePosToRay(out set<Object> collisions, Object ignore = null, float raycast_distance = 3000, float radius = 0, bool groundonly = false)
{
	vector ray_start = GetGame().GetCurrentCameraPosition();
	vector ray_end = ray_start + GetGame().GetPointerDirection() * raycast_distance;
	
	vector hitPos, hitNormal;
	int hitComponentIndex;		
	collisions = new set<Object>;

	
	// todo: collision mode is very inaccurate atm due to RayCastBullet crashing when dragging dynamic objects
	// find a way to check if the object is dynamic, then bop it
	//Object hit_object;
	//float hit_else;
	//if (!groundonly && DayZPhysics.RayCastBullet(ray_start, ray_end, PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.BUILDING | PhxInteractionLayers.CHARACTER, ignore, hit_object, hitPos, hitNormal, hit_else)) {
	//	collisions.Insert(hit_object);
	//	return hitPos;
	//}
	
	DayZPhysics.RaycastRV(ray_start, ray_end, hitPos, hitNormal, hitComponentIndex, collisions, null, ignore, false, groundonly, 1, radius, CollisionFlags.ALLOBJECTS);
	
	
	return hitPos;
}

static Object GetObjectUnderCursor(float raycast_distance = 3000)
{
	vector ray_start = GetGame().GetCurrentCameraPosition();
	vector ray_end = ray_start + GetGame().GetPointerDirection() * raycast_distance;
	
	vector hitPos, hitNormal;
	int hitComponentIndex;		
	set<Object> collisions = new set<Object>;
	

	DayZPhysics.RaycastRV(ray_start, ray_end, hitPos, hitNormal, hitComponentIndex, collisions);
	
	return collisions.Get(0);
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


