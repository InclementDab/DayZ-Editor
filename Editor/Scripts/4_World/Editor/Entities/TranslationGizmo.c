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
		m_RayViews.Insert(new RayView(COLOR_RED));
		
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
		
		vector gizmo_local_transform[4];
		Math3D.MatrixInvMultiply4(camera_transform, gizmo_transform, gizmo_local_transform);

		//aPrint(inv_transform);
		vector angles = Math3D.MatrixToAngles(gizmo_local_transform);		
		
		vector start_point = GetGame().GetScreenPos(gizmo_transform[3]);
		vector end_point = GetGame().GetScreenPos(GetMemoryPointPos("x").Multiply4(gizmo_transform));
		vector delta = end_point - start_point;

		//ray_view.GetLayoutRoot().SetRotation(angles[0], angles[1], angles[2]);

		Math3D.MatrixMultiply4(camera_transform, gizmo_local_transform, gizmo_local_transform);
		gizmo_local_transform[3] = Vector(0, 0, 1).Multiply4(camera_transform);
		
		
		Shape.CreateMatrix(gizmo_local_transform);
		
		
		float dist = vector.Distance(gizmo_transform[3], camera_transform[3]);

		//ray_view.GetLayoutRoot().SetPos(start_point[0], start_point[1]);
		//ray_view.GetLayoutRoot().SetSize(delta.Length(), 24);
	}
}