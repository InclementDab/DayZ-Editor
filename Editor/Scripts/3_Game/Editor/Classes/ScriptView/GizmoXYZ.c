class GizmoXYZ: ScriptView
{
	protected ObjectNode m_ObjectNode;
	
	protected vector m_SavedAngles;
	
	Widget X, Y, Z;
	
	void GizmoXYZ(ObjectNode object_node)
	{
		m_ObjectNode = object_node;
	}
		
	override void Update(float dt)
	{	
		vector gizmo_transform[4] = {};
		m_ObjectNode.GetObject().GetTransform(gizmo_transform);
		gizmo_transform[3] = m_ObjectNode.GetTopPoint();
		Shape.CreateMatrix(gizmo_transform);	
		
		Camera camera = GetDayZGame().GetSandbox().GetEditor().GetCamera();
		if (!camera) {
			return;
		}
		
		vector camera_transform[4];
		camera.GetTransform(camera_transform);
			
							
		float distance_to_target = vector.Distance(gizmo_transform[3], gizmo_transform[0].Multiply4(gizmo_transform[3]));								
		float x, y;
		GetLayoutRoot().GetScreenSize(x, y);
		
		//float screen_size = 2 * Math.Tan(GetDayZGame().GetEditor().GetCamera().GetCurrentFOV() / 2) * gizmo_transform[0].Length();			
		
		vector ident[4];
		Math3D.MatrixIdentity4(ident);
	
		//Math3D.MatrixInvMultiply4(camera_transform, matrix, matrix);
		
		
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		float aspect_ratio = (float)screen_x / (float)screen_y;
		float scale_y = 1 / Math.Tan(camera.GetCurrentFOV() / 2);
		float scale_x = scale_y / aspect_ratio;
		
		float far_plane = 150.0;
		float near_plane = camera.GetNearPlane();
		float frustum = far_plane - near_plane;
		float zz = -(far_plane + near_plane) / frustum;
		float zw = -2 * (far_plane * near_plane) / frustum;
		
		vector perspective_matrix[4] = {
			Vector(scale_x, 0, 0),
			Vector(0, scale_y, -1),
			Vector(0, 0, zz),
			Vector(0, 0, zw),
		};
			
		vector matrix[4];
		Math3D.MatrixInvMultiply4(camera_transform, ident, matrix);
		Math3D.MatrixMultiply3(matrix, gizmo_transform, matrix);
		Math3D.MatrixMultiply3(matrix, perspective_matrix, matrix);
		
		vector angles = Math3D.MatrixToAngles(matrix);
		X.SetSort(1);
		Y.SetSort(1);
		
		DbgUI.Text(angles.ToString());
				
		m_LayoutRoot.SetRotation(angles[1], 0,0);
		
		//GetLayoutRoot().SetRotation(m_SavedAngles[0], m_SavedAngles[1], m_SavedAngles[2]);
	
		float height = 24;
		//m_RayView.GetLayoutRoot().SetSize(width, height);
		
		vector screen_origin = GetGame().GetScreenPos(gizmo_transform[3]);
		screen_origin[2] = 0;
		GetLayoutRoot().SetScreenPos(screen_origin[0], screen_origin[1]);
		GetLayoutRoot().Show(screen_origin[2] > 0);
		GetLayoutRoot().Update();
	}
		
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Gizmo\\XYZ.layout";
	}
}