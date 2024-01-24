class RayView: ScriptView
{
	protected int m_Color;
	
	Widget Body, Arrow;
	
	void RayView(int color)
	{
		m_Color = color;
		
		Body.SetColor(color);
		Arrow.SetColor(color);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.AnimateColor(Body, COLOR_YELLOW, 100);
		WidgetAnimator.AnimateColor(Arrow, COLOR_YELLOW, 100);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		WidgetAnimator.AnimateColor(Body, m_Color, 100);
		WidgetAnimator.AnimateColor(Arrow, m_Color, 100);
		
		return super.OnMouseLeave(w, enterW, x, y);
	} 
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\ray.layout";
	}
}

class TranslationGizmo: House
{
	protected ref array<ref RayView> m_RayViews = {};
	
	void TranslationGizmo()
	{
		m_RayViews.Insert(new RayView(COLOR_GREEN));
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(OnUpdate);
	}
	
	void ~TranslationGizmo()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(OnUpdate);
	}
	
	void OnUpdate(float dt)
	{
		if (GetGame().IsDedicatedServer()) {
			return;
		}
		
		vector gizmo_transform[4];
		GetRenderTransform(gizmo_transform);

		vector camera_transform[4];
		GetDayZGame().GetEditor().GetCamera().GetTransform(camera_transform);
				
		vector matrix[4];
		Math3D.MatrixInvMultiply4(camera_transform, gizmo_transform, matrix);
		
		vector angle_matrix[3];
		Math3D.YawPitchRollMatrix(Vector(0, 90, 0), angle_matrix);
		
		Math3D.MatrixMultiply3(matrix, angle_matrix, matrix);
		
		//aPrint(inv_transform);
		vector angles = Math3D.MatrixToAngles(matrix);		
		
		Shape.CreateMatrix(gizmo_transform);
		
		ClearFlags(EntityFlags.VISIBLE, true);
		
		Debug.DrawSphere(gizmo_transform[3], 0.01, COLOR_RED, ShapeFlags.ONCE);
		vector start_point = GetGame().GetScreenPos(gizmo_transform[3]);
		vector end_point = GetGame().GetScreenPos(vector.Up.Multiply4(matrix));
		
		vector delta = Vector(end_point[0], end_point[1], 0) - Vector(start_point[0], start_point[1], 0);
		RayView ray_view = m_RayViews[0];
		
		
		//ray_view.GetLayoutRoot().SetTransform(gizmo_transform);
		
		int x, y;
		GetScreenSize(x, y);
		ray_view.GetLayoutRoot().SetPos(start_point[0], start_point[1]);
		ray_view.GetLayoutRoot().SetRotation(angles[0], angles[1], angles[2]);
		ray_view.GetLayoutRoot().SetSize(delta.Length(), 24);
	}
}