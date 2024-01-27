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
		GetTransform(gizmo_transform);

		vector camera_transform[4];
		GetDayZGame().GetEditor().GetCamera().GetTransform(camera_transform);
		
		vector identity_matrix[4];
		Math3D.MatrixIdentity4(identity_matrix);
		
		vector matrix[4];
		Math3D.MatrixInvMultiply4(camera_transform, identity_matrix, matrix);
		
		Math3D.MatrixMultiply4(matrix, camera_transform, matrix);
		
		//vector angle_matrix[3];
		//Math3D.YawPitchRollMatrix(Vector(0, 90, 0), angle_matrix);
		
		//Math3D.MatrixMultiply3(matrix, angle_matrix, matrix);
		
		//aPrint(inv_transform);
		//vector angles = Math3D.MatrixToAngles(matrix);		
		
		Shape.CreateMatrix(gizmo_transform);
		
		ClearFlags(EntityFlags.VISIBLE, true);
		
		Debug.DrawSphere(gizmo_transform[3], 0.01, COLOR_RED, ShapeFlags.ONCE);		
		vector screen_origin = GetGame().GetScreenPos(gizmo_transform[3]);
		m_RayViews.Clear();		
		for (int i = 0; i < 3; i++) {
			
			Shape.CreateSphere(COLOR_GREEN, ShapeFlags.TRANSP | ShapeFlags.ONCE, gizmo_transform[i].Multiply4(gizmo_transform[3]), 0.01);
			vector v = vector.Zero;
			v[i] = 1.0;
			vector screen_end = GetGame().GetScreenPos(v.Multiply4(gizmo_transform[3]));
			//Print(v.Multiply4(gizmo_transform[3]));
			
			float distance_to_target = vector.Distance(gizmo_transform[3], gizmo_transform[i].Multiply4(gizmo_transform[3]));
			//Print(distance_to_target);
			//Shape.CreateSphere(ARGB(200, 255, 255, 255), ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.NOZWRITE, origin, 0.025);
			//Shape.CreateSphere(ARGB(200, 255, 255, 255), ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.NOZWRITE, end, 0.025);
						
			// Size of 1m pixels in X direction
			//float uniform_pixel_size = 1 / vector.Distance(Vector(screen_end[0], screen_end[1], 0), Vector(screen_origin[0], screen_origin[1], 0));

			int color = COLOR_RED;
			if (i == 1) { color = COLOR_GREEN; };
			if (i == 2) { color = COLOR_BLUE; };
			
			RayView ray_view = new RayView(color);
			m_RayViews.Insert(ray_view);
			
			//GizmoOriginView gizmo_origin = new GizmoOriginView();
			//m_RayViews.Insert(gizmo_origin);
								
			float x, y;
			ray_view.GetLayoutRoot().GetScreenSize(x, y);
			
			float screen_size = 2 * Math.Tan(GetDayZGame().GetEditor().GetCamera().GetCurrentFOV() / 2) * gizmo_transform[i].Length();			
			
			ray_view.GetLayoutRoot().SetScreenPos(screen_end[0], screen_end[1]);
			
			vector angles = Math3D.MatrixToAngles(matrix);
			
			ray_view.GetLayoutRoot().SetRotation(angles[0], angles[1], angles[2]);
			//Print(uniform_pixel_size);
			
			ray_view.GetLayoutRoot().SetSize(screen_size * distance_to_target, 24 * distance_to_target);
		}
	}
}