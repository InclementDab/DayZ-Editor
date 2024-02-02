class Ray: Managed
{
	vector Position;
	vector Direction;
	
	void Ray(vector position = vector.Zero, vector direction = vector.Zero)
	{
		Position = position;
		Direction = direction;
	}
	
	vector GetPoint(float distance)
	{
		return Position + Direction * distance;
	}
	
	Raycast PerformRaycast(Object ignore = null)
	{
		Ray output_ray = new Ray();
		Object hit_object;
		
		float fraction;
		if (!DayZPhysics.RayCastBullet(Position, Position + Direction * 1000.0, PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.CHARACTER | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR, ignore, hit_object, output_ray.Position, output_ray.Direction, fraction)) {			
			return null;
		}
				
		Raycast raycast = new Raycast();
		raycast.Source = this;
		raycast.Bounce = output_ray;
		raycast.Hit = hit_object;		
		return raycast;
	}
	
	Raycast PerformRaycastRV(Object ignore = null, Object with = null, float radius = 0.0)
	{
		Ray output_ray = new Ray();
		
		set<Object> rv_results = new set<Object>();
		int hit_component;
		if (!DayZPhysics.RaycastRV(Position, Position + Direction * 100.0, output_ray.Position, output_ray.Direction, hit_component, rv_results, with, ignore, false, false, ObjIntersectFire, radius)) {
			return null;
		}
		
		Raycast raycast = new Raycast();
		raycast.Source = this;
		raycast.Bounce = output_ray;
		raycast.Hit = rv_results[0];	
		raycast.HitComponent = hit_component;
		return raycast;
	}
	
	void Debug()
	{
		Shape.CreateArrow(Position, GetPoint(0.5), 1.0, COLOR_BLUE, ShapeFlags.ONCE);
	}
}