static vector MousePosToRay(out set<Object> collisions, Object ignore = null, float raycast_distance = 3000, float radius = 0, bool groundonly = false)
{
	vector ray_start = GetGame().GetCurrentCameraPosition();
	vector ray_end = ray_start + GetGame().GetPointerDirection() * raycast_distance;
	
	vector hitPos, hitNormal;
	int hitComponentIndex;		
	collisions = new set<Object>;

	Object hit_object;
	float hit_else;
	int interaction_layers = PhxInteractionLayers.BUILDING;
	if (ignore && dBodyIsDynamic(ignore)) {
		interaction_layers = dBodyGetInteractionLayer(ignore);
	}
	
	if (!groundonly && ignore && dBodyIsDynamic(ignore) && DayZPhysics.RayCastBullet(ray_start, ray_end, interaction_layers, ignore, hit_object, hitPos, hitNormal, hit_else)) {
		collisions.Insert(hit_object);
		return hitPos;
	}
	
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
	for (int i = 0; i < 3; i++) {
		result[i] = Math.AbsFloat(size[0][i]) + Math.AbsFloat(size[1][i]);
	}
	
	return result;
}


