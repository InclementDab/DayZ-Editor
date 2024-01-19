class TranslationGizmo: House
{
	void TranslationGizmo()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(OnUpdate);
	}
	
	void ~TranslationGizmo()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(OnUpdate);
	}
	
	void OnUpdate(float dt)
	{
		if (GetGame().IsDedicatedServer()) {
			return;
		}
		
		vector ray_start = GetGame().GetCurrentCameraPosition();
		vector ray_end = ray_start + GetGame().GetCurrentCameraDirection() * 3000;
		Object hit_object;
		vector position, normal;
		float fraction;
		
		
		//DayZPhysics.RayCastBullet(ray_start, ray_end, PhxInteractionLayers.DEFAULT, null, hit_object, position, normal, fraction);
		//Print(hit_object);		
	}
}