class Plane: Managed
{
	vector Corner[2], Normal, Forward;
	
	void Plane(vector corner0, vector corner1, vector normal, vector forward)
	{
		Corner[0] = corner0;
		Corner[1] = corner1;
		Normal = normal.Normalized();
		Forward = forward.Normalized();
		if (vector.Dot(Normal, Forward) != 0) {
			Error("Non orthongonal basis vectors used");
			return;
		}
	}
	
	vector GetPosition()
	{
		return (Corner[0] + Corner[1]) * 0.5;
	}
	
	vector GetSize()
	{
		return (Corner[1] - Corner[0]) * 0.5;
	}
				
	void Debug(string name, vector mat[4])
	{		
		vector plane_matrix[4] = {
			Forward * Normal,
			Normal,
			Forward,
			GetPosition()
		};
		
		Math3D.MatrixMultiply4(mat, plane_matrix, plane_matrix);
		Shape.CreateMatrix(plane_matrix);
					
		vector p[2] = { Corner[0].Multiply4(mat), Corner[1].Multiply4(mat) };
		Shape.CreateLines(COLOR_WHITE, ShapeFlags.ONCE | ShapeFlags.DOUBLESIDE, p, 2);
		
		Shape bbox = Shape.Create(ShapeType.BBOX, COLOR_RED_A, ShapeFlags.ONCE | ShapeFlags.TRANSP, Corner[0], Corner[1]);
		bbox.SetMatrix(mat);
		
		GetDayZGame().DebugDrawText(name, GetPosition().Multiply4(mat), 1);
		
		Shape.CreateSphere(COLOR_PALE_B, ShapeFlags.DOUBLESIDE | ShapeFlags.ONCE, Corner[0].Multiply4(mat), 0.05);
		Shape.CreateSphere(COLOR_PALE_B, ShapeFlags.DOUBLESIDE | ShapeFlags.ONCE, Corner[1].Multiply4(mat), 0.05);
	}
}