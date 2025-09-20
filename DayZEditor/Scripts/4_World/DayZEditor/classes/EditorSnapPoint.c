class EditorSnapPoint: Managed
{
	protected EditorWorldObject m_EditorObject;
	protected ref array<ref EditorSnapPoint> m_SnapPoints = {};
	
	vector Transform[4];
	bool AllowReverse = false;
	
	void EditorSnapPoint(notnull EditorWorldObject editor_object)
	{
		m_EditorObject = editor_object;
	}
	
	void GetTransformLS(out vector mat[4])
	{
		copyarray(mat, Transform);
	}
	
	void GetTransformWS(out vector mat[4])
	{
		vector editor_object_mat[4];
		m_EditorObject.GetTransform(editor_object_mat);
		Math3D.MatrixMultiply4(editor_object_mat, Transform, mat);
	}
	
	EditorWorldObject GetEditorObject()
	{
		return m_EditorObject;
	}
	
	vector GetWorldPosition()
	{
		vector editor_object_mat[4];
		m_EditorObject.GetTransform(editor_object_mat);
		return Transform[3].Multiply4(editor_object_mat);
	}
	
	vector GetDirectionAside()
	{
		vector editor_object_mat[4];
		m_EditorObject.GetTransform(editor_object_mat);
		return Transform[0].Multiply3(editor_object_mat);
	}
	
	vector GetDirectionUp()
	{
		vector editor_object_mat[4];
		m_EditorObject.GetTransform(editor_object_mat);
		return Transform[1].Multiply3(editor_object_mat);
	}
	
	vector GetDirectionForward()
	{
		vector editor_object_mat[4];
		m_EditorObject.GetTransform(editor_object_mat);
		return Transform[2].Multiply3(editor_object_mat);
	}
	
	bool IsValidSnap(notnull EditorSnapPoint rhs)
	{
		return true;
	}
	
	void SnapTo(notnull EditorSnapPoint anchor_point, out vector mat[4], bool reverse)
	{		
		vector anchor_world_transform[4];
		anchor_point.GetTransformWS(anchor_world_transform);
		Shape.CreateMatrix(anchor_world_transform);	
		vector ident[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			vector.Zero
		};
		
		// Flipping the matrix first
		vector matrix_reverser[4];
		Math3D.YawPitchRollMatrix(Vector(0, 180 + reverse * 180, 180 + reverse * 180), matrix_reverser);
		
		vector coupling_matrix[4];
		Math3D.MatrixMultiply4(anchor_world_transform, matrix_reverser, coupling_matrix);
		
		vector source_inverse[4];	
		Math3D.MatrixInvMultiply4(Transform, ident, source_inverse);
		Math3D.MatrixMultiply4(coupling_matrix, source_inverse, mat);
	}
	
	static array<EditorSnapPoint> GetSnapPointsInRange(notnull Object target_object, float radius)
	{
		array<Object> objects = {};
		GetGame().GetObjectsAtPosition3D(target_object.GetPosition(), radius, objects, null);
		
		array<EditorSnapPoint> snap_points = {};
		
		foreach (Object object: objects) {
			EditorObject editor_object = GetEditor().GetEditorObject(object);
			if (editor_object) {
				array<ref EditorSnapPoint> snap_points2 = editor_object.GetSnapPoints();
				foreach (EditorSnapPoint snap_point: snap_points2) {
					snap_points.Insert(snap_point);
				}
			}
		}

		return snap_points;
	}
}

class EditorSnapPlane: Managed
{
	protected EditorWorldObject m_EditorObject;
	
	ref EditorSnapPoint m_SnapPoint;
	ref Plane m_Plane;
	vector m_Aside;
	vector m_Clipping[2];
	
	void EditorSnapPlane(notnull EditorWorldObject world_object, notnull Plane plane, vector clipping[2])
	{
		m_EditorObject = world_object;
		m_Plane = plane;
		copyarray(m_Clipping, clipping);
		m_SnapPoint = new EditorSnapPoint(world_object);
				
		m_SnapPoint.Transform[3] = plane.Position;
	}
	
	vector GetWorldPosition()
	{
		vector editor_object_mat[4];
		m_EditorObject.GetTransform(editor_object_mat);
		return m_Plane.Position.Multiply4(editor_object_mat);
	}
	
	vector GetWorldNormal()
	{
		vector editor_object_mat[4];
		m_EditorObject.GetTransform(editor_object_mat);
		return m_Plane.Normal.Multiply3(editor_object_mat);
	}
	
	void ToMatrix(out vector mat[4])
	{
		vector aside = m_Plane.GetAside();
		vector mat_local[4] = {
			aside,
			m_Plane.Normal,
			aside * m_Plane.Normal,
			m_Plane.Position
		};
				
		Math3D.MatrixOrthogonalize4(mat_local);		
		vector editor_object_mat[4];
		m_EditorObject.GetTransform(editor_object_mat);
		Math3D.MatrixMultiply4(editor_object_mat, mat_local, mat);
	}
	
	void Debug(LinearColor color = 0x1fff7f7f, ShapeFlags flags = ShapeFlags.TRANSP | ShapeFlags.ADDITIVE | ShapeFlags.ONCE)
	{
#ifdef DIAG_DEVELOPER
		vector plane_matrix[4];
		ToMatrix(plane_matrix);
		vector editor_object_mat[4];
		m_EditorObject.GetTransform(editor_object_mat);
		
		vector bounding_center = m_EditorObject.GetClippingCenter();

		Shape.CreateMatrix(plane_matrix, 1);		
		Shape shape = Shape.Create(ShapeType.BBOX, color, flags, m_Clipping[0], m_Clipping[1]);
		
		shape.SetMatrix(plane_matrix);
		Debug.AddShape(shape, flags);
		
		//shape = Shape.CreateSphere(-1, ShapeFlags.ONCE, bounding_center, 0.1);
		//shape.SetMatrix(editor_object_mat);
#endif
	}
}