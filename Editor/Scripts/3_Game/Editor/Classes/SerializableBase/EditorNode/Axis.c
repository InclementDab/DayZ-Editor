class Axis: int
{
	static const Axis BOTTOM = 0;
	static const Axis LEFT = 1;
	static const Axis FRONT = 2;
	static const Axis RIGHT = 3;
	static const Axis BACK = 4;
	static const Axis TOP = 5;

	static const ref array<Axis> ALL = { BOTTOM, LEFT, FRONT, RIGHT, BACK, TOP };
		
	static void CreateMatrix(Axis axis, out vector mat[4])
	{
		mat = {
			axis.Forward(),
			axis.Up(),
			axis.Forward() * axis.Up()
		};
	}
	
	vector Forward()
	{
		switch (value) {
			case BOTTOM: return vector.Aside * -1;
			case TOP: return vector.Aside;
			case LEFT: 
			case RIGHT:
			case BACK:
			case FRONT: return vector.Up; // Return Up on side faces, i think
		}
		
		return vector.Zero;
	}
	
	vector Up()
	{
		switch (value) {
			case BOTTOM: return vector.Up * -1;
			case LEFT: return vector.Aside * -1;
			case BACK: return vector.Forward * -1;
			case TOP: return vector.Up;
			case RIGHT: return vector.Aside;
			case FRONT: return vector.Forward;
		}
		
		return vector.Zero;
	}
	
	Plane CreatePlane(vector corners[2])
	{
		return new Plane(corners[0], corners[1], value.Up(), value.Forward());
	}
}

typedef int Axis;