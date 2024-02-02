class GizmoXYZ: ScriptView
{
	protected ObjectNode m_ObjectNode;
	
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
		
		vector screen_origin = GetGame().GetScreenPos(gizmo_transform[3]);
		
		if (screen_origin[2] < 0) {
			//return;
		}
		
		screen_origin[2] = 0;
							
		float distance_to_target = vector.Distance(gizmo_transform[3], gizmo_transform[0].Multiply4(gizmo_transform[3]));								
		float x, y;
		GetLayoutRoot().GetScreenSize(x, y);
		
		//float screen_size = 2 * Math.Tan(GetDayZGame().GetEditor().GetCamera().GetCurrentFOV() / 2) * gizmo_transform[0].Length();			
		
		vector matrix[3];
		Math3D.MatrixInvMultiply3(gizmo_transform, camera_transform, matrix);
		//Math3D.MatrixMultiply3(camera_transform, matrix, matrix);
		
		vector ypr_matrix[3];
		
		Math3D.YawPitchRollMatrix(Vector(-90 * GetGame().GetInput().LocalValue_ID(UAFire), -90 * GetGame().GetInput().LocalValue_ID(UATempRaiseWeapon), 90), ypr_matrix);
		Math3D.MatrixMultiply3(matrix, ypr_matrix, matrix);
		
		vector angles = Math3D.MatrixToAngles(matrix);
		GetLayoutRoot().SetRotation(angles[0], angles[1], angles[2]);
				
		float height = 24;
		//m_RayView.GetLayoutRoot().SetSize(width, height);
		GetLayoutRoot().SetScreenPos(screen_origin[0], screen_origin[1]);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		//WidgetAnimator.AnimateColor(Body, COLOR_YELLOW, 100);
		//WidgetAnimator.AnimateColor(Arrow, COLOR_YELLOW, 100);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//WidgetAnimator.AnimateColor(Body, m_Color, 100);
		//WidgetAnimator.AnimateColor(Arrow, m_Color, 100);
		
		return super.OnMouseLeave(w, enterW, x, y);
	} 
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Gizmo\\Ray.layout";
	}
}