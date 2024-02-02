class GizmoXYZ: ScriptView
{
	protected ObjectNode m_ObjectNode;
	
	protected vector m_SavedAngles;
	
	void GizmoXYZ(ObjectNode object_node)
	{
		m_ObjectNode = object_node;
	}
	
	override void Update(float dt)
	{	
		vector gizmo_transform[4] = {};
		m_ObjectNode.GetObject().GetTransform(gizmo_transform);
		gizmo_transform[3] = m_ObjectNode.GetTopPoint();

		vector camera_transform[4];
		m_ObjectNode.GetEditor().GetCamera().GetTransform(camera_transform);
		
		Shape.CreateMatrix(gizmo_transform);
		
		return;
							
		float distance_to_target = vector.Distance(gizmo_transform[3], gizmo_transform[0].Multiply4(gizmo_transform[3]));								
		float x, y;
		GetLayoutRoot().GetScreenSize(x, y);
		
		//float screen_size = 2 * Math.Tan(GetDayZGame().GetEditor().GetCamera().GetCurrentFOV() / 2) * gizmo_transform[0].Length();			
		
		vector test[3] = {
			vector.Aside,
			vector.Up,
			vector.Forward
		};
		
		//Math3D.MatrixMultiply3(gizmo_transform, test, gizmo_transform);
		
		vector identity_matrix[3];
		Math3D.MatrixIdentity3(identity_matrix);
		
		vector matrix[4];
		Math3D.MatrixInvMultiply4(camera_transform, identity_matrix, matrix);
		
		
		vector ypr_matrix[3];
		//Math3D.YawPitchRollMatrix(m_SavedAngles, ypr_matrix);
		//Math3D.MatrixMultiply3(ypr_matrix, matrix, matrix);
		
		vector angles = Math3D.MatrixToAngles(matrix);
		
		
		DbgUI.Text(angles.ToString());
		
		m_LayoutRoot.SetRotation(angles[1], -angles[0], 0);
		
		//GetLayoutRoot().SetRotation(m_SavedAngles[0], m_SavedAngles[1], m_SavedAngles[2]);
				
		float height = 24;
		//m_RayView.GetLayoutRoot().SetSize(width, height);
		
		vector screen_origin = GetGame().GetScreenPos(gizmo_transform[3]);
		
		if (screen_origin[2] < 0) {
			//return;
		}
		
		screen_origin[2] = 0;
		GetLayoutRoot().SetScreenPos(screen_origin[0], screen_origin[1]);
		GetLayoutRoot().Update();
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Gizmo\\XYZ.layout";
	}
}