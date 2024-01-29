class Ray: Managed
{
	static const ref Ray INVALID = new Ray();	
	
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
	
	void Debug()
	{
		Shape.CreateArrow(Position, GetPoint(0.5), 1.0, COLOR_BLUE, ShapeFlags.ONCE);
	}
}