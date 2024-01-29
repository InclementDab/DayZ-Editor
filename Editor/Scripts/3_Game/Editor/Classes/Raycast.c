class Raycast: Managed
{
	ref Ray Source;
	ref Ray Bounce;
	
	Object Hit;
	int HitComponent;
	
	void Debug()
	{
		if (Hit) {
			GetDayZGame().DebugDrawText(string.Format("Hit: %1 %2", Hit.GetType(), Bounce.Position), Hit.GetPosition(), 1.0);
		}
		
		Bounce.Debug();
	}
}