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
		return "Editor\\GUI\\layouts\\Gizmo\\Ray.layout";
	}
}

class GizmoOriginView: ScriptView
{
	Widget Body;
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.AnimateColor(Body, COLOR_YELLOW, 100);
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		WidgetAnimator.AnimateColor(Body, COLOR_WHITE, 100);		
		return super.OnMouseLeave(w, enterW, x, y);
	} 
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Gizmo\\Origin.layout";
	}
}

class TranslationGizmo: House
{
	protected ref array<ref ScriptView> m_RayViews = {};
	
	void TranslationGizmo()
	{
		//m_RayViews.Insert(new RayView(COLOR_GREEN));
		
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
		vector origin = gizmo_transform[3];
		vector end = Vector(0.5, 0, 0).Multiply4(gizmo_transform);
		
		Shape.CreateSphere(ARGB(200, 255, 255, 255), ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.NOZWRITE, origin, 0.025);
		Shape.CreateSphere(ARGB(200, 255, 255, 255), ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.NOZWRITE, end, 0.025);
		
		vector screen_origin = GetGame().GetScreenPos(origin);
		vector screen_end = GetGame().GetScreenPos(end);
				
		RayView ray_view = new RayView(COLOR_RED);
		GizmoOriginView gizmo_origin = new GizmoOriginView();
		
		m_RayViews.Clear();
		m_RayViews.Insert(ray_view);
		m_RayViews.Insert(gizmo_origin);
		
		//Print(GetDayZGame().GetEditor().GetCamera().GetCurrentFOV() * Math.RAD2DEG);
		
		
	
		float x, y;
		ray_view.GetLayoutRoot().GetScreenSize(x, y);
		
		ray_view.GetLayoutRoot().SetScreenPos(screen_origin[0], screen_origin[1] - (y / 2));
		
		
		vector delta = screen_end - screen_origin;
		delta[2] = 0;

		ray_view.GetLayoutRoot().SetRotation(0, 0, 180);
		ray_view.GetLayoutRoot().SetSize(delta.Length(), 24);
		
		
		gizmo_origin.GetLayoutRoot().GetScreenSize(x, y);
		gizmo_origin.GetLayoutRoot().SetPos(screen_origin[0] - (x / 2), screen_origin[1] - (y / 2));
	}
}