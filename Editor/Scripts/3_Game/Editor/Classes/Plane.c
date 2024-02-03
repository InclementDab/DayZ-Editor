class Plane: Managed
{
	vector Corner[2], Normal, Aside;
	
	void Plane(vector corner0, vector corner1, vector normal, vector aside)
	{
		Corner[0] = corner0;
		Corner[1] = corner1;
		Normal = normal.Normalized();
		Aside = aside.Normalized();
		if (vector.Dot(Normal, Aside) != 0) {
			//Error("Non orthongonal basis vectors used");
			return;
		}
	}
	
	static Plane Create(vector normal, vector size, vector position, vector aside)
	{
		if (vector.Dot(normal, aside) != 0) {
			PrintFormat("Non orthongonal basis vectors used %1", vector.Dot(normal, aside));
		}
		
		vector matrix[3];
		Math3D.DirectionAndUpMatrix(aside, normal, matrix);
		size = size * 0.5;
		size[2] = 0;
		vector corner0 = size.Multiply3(matrix);
		vector corner1 = (-size).Multiply3(matrix);
				
		return new Plane(corner0, corner1, normal, aside);
	}
	
	void CreateMatrix(out vector mat[4])
	{
		mat = {
			Aside,
			Normal,
			Aside * Normal,
			(Corner[0] + Corner[1]) * 0.5
		};
				
		Math3D.MatrixOrthogonalize4(mat);
	}
	
	vector GetPosition()
	{
		return (Corner[0] + Corner[1]) * 0.5;
	}
	
	vector GetSize()
	{
		return (Corner[1] - Corner[0]) * 0.5;
	}
	
	vector Intersect(notnull Ray source, vector mat[4])
    {
        float d1 = vector.Dot(Normal.Multiply3(mat), source.Position - GetPosition().Multiply4(mat));
		float d2 = vector.Dot(Normal.Multiply3(mat), -source.Direction);
		if (d2 == 0) {
			return mat[3];
		}
		
        return source.Position + (d1 / d2) * source.Direction;
    }
			
	private Object _debug;
		
	void Debug(string name, vector mat[4])
	{		
		vector plane_matrix[4];
		CreateMatrix(plane_matrix);
		
		Math3D.MatrixMultiply4(mat, plane_matrix, plane_matrix);
		
		Shape.CreateMatrix(plane_matrix);
					
		vector p[2] = { Corner[0].Multiply4(mat), Corner[1].Multiply4(mat) };
		Shape.CreateLines(COLOR_WHITE, ShapeFlags.ONCE, p, 2);
		
		Shape bbox = Shape.Create(ShapeType.BBOX, COLOR_RED_A, ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.ADDITIVE, Corner[0], Corner[1]);
		bbox.SetMatrix(mat);
		
		GetDayZGame().DebugDrawText(name, GetPosition().Multiply4(mat), 1);
		
		Shape.CreateSphere(COLOR_PALE_B, ShapeFlags.DOUBLESIDE | ShapeFlags.ONCE, Corner[0].Multiply4(mat), 0.05);
		Shape.CreateSphere(COLOR_PALE_B, ShapeFlags.DOUBLESIDE | ShapeFlags.ONCE, Corner[1].Multiply4(mat), 0.05);
	}
}