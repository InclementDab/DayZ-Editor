
static vector GetObjectSize(Object obj)
{
	vector size[2];
	vector result;
	
	float radius = obj.ClippingInfo(size);
	
	result[0] = Math.AbsFloat(size[0][0]) + Math.AbsFloat(size[1][0]);
	result[1] = Math.AbsFloat(size[0][1]) + Math.AbsFloat(size[1][1]);
	result[2] = Math.AbsFloat(size[0][2]) + Math.AbsFloat(size[1][2]);
	return result;
}

static vector MousePosToRay(out set<Object> collisions = null, Object ignore = null, float raycast_distance = 1000)
{
	vector rayStart = GetGame().GetCurrentCameraPosition();
	vector rayEnd = rayStart + GetGame().GetPointerDirection() * raycast_distance;
	vector hitPos, hitNormal;
	int hitComponentIndex;		
	collisions = new set<Object>;

	DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, collisions, NULL, ignore, false, false, 1, 0, CollisionFlags.FIRSTCONTACT);
	return hitPos;
}




static vector AverageVectors(vector v1, vector v2)
{
	vector result;
	
	result[0] = (v1[0] + v2[0])/2;
	result[1] = (v1[1] + v2[1])/2;
	result[2] = (v1[2] + v2[2])/2;
	
	return result;
}

static vector DivideVectorByNumber(vector v1, float v2)
{
	vector result;
	
	result[0] = v1[0]/2;
	result[1] = v1[1]/2;
	result[2] = v1[2]/2;
	
	return result;
}


static vector DivideVector(vector v1, vector v2)
{
	vector result;
	
	result[0] = v1[0]/v2[0];
	result[1] = v1[1]/v2[1];
	result[2] = v1[2]/v2[2];
	
	return result;
}
