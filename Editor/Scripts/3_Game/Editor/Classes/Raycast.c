class Raycast: Managed
{
	static const ref Raycast INVALID = new Raycast();
	
	ref Ray Source = Ray.INVALID;
	ref Ray Bounce = Ray.INVALID;
	
	Object Hit;
	
	void Debug()
	{
		Bounce.Debug();
	}
}