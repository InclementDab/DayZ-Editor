static vector MousePosToRay(out set<Object> collisions, Object ignore = null, float raycast_distance = 3000, float radius = 0, bool groundonly = false, bool highprecision = false)
{
	vector ray_start = GetGame().GetCurrentCameraPosition();
	vector ray_end = ray_start + GetGame().GetPointerDirection() * raycast_distance;
	
	vector hitPos, hitNormal;
	int hitComponentIndex;		
	collisions = new set<Object>;

	Object hit_object;
	float hit_else;
	//int interaction_layers = PhxInteractionLayers.FIREGEOM;
	int interaction_layers = PhxInteractionLayers.BUILDING | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FIREGEOM;
	//int interaction_layers = PhxInteractionLayers.NOCOLLISION | PhxInteractionLayers.DEFAULT | PhxInteractionLayers.BUILDING | PhxInteractionLayers.CHARACTER | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.VEHICLE_NOTERRAIN | PhxInteractionLayers.CHARACTER_NO_GRAVITY | PhxInteractionLayers.FIREGEOM | PhxInteractionLayers.DOOR | PhxInteractionLayers.RAGDOLL | PhxInteractionLayers.WATERLAYER | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.GHOST | PhxInteractionLayers.WORLDBOUNDS | PhxInteractionLayers.FENCE | PhxInteractionLayers.AI | PhxInteractionLayers.AI_NO_COLLISION | PhxInteractionLayers.AI_COMPLEX | PhxInteractionLayers.TINYCAPSULE | PhxInteractionLayers.TRIGGER | PhxInteractionLayers.TRIGGER_NOTERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.CAMERA | PhxInteractionLayers.TEMP;
	/*if (ignore && dBodyIsDynamic(ignore)) {
		interaction_layers = dBodyGetInteractionLayer(ignore);
	}*/
	/*
	if (!groundonly && ignore) {
		PrintFormat("Testing %1 - Dynamic: %2", ignore, dBodyIsDynamic(ignore));
		
		for (int i = PhxInteractionLayers.NOCOLLISION; i <= PhxInteractionLayers.TEMP; i++) {
			if ((dBodyGetInteractionLayer(ignore) & i) == i) {
				PrintFormat("	%1 has interaction layer %2", ignore, typename.EnumToString(PhxInteractionLayers, i));
			}
		}
	}*/
	
	// groundonly && ignore && dBodyIsDynamic(ignore) &&
	if (!groundonly && highprecision && DayZPhysics.RayCastBullet(ray_start, ray_end, interaction_layers, ignore, hit_object, hitPos, hitNormal, hit_else)) {
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