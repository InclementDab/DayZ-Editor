class RayView: ScriptView
{
	protected int m_Color;
	protected vector m_Direction;
	
	Widget Body, Arrow;
	
	void RayView(int color, vector direction)
	{
		m_Color = color;
		m_Direction = direction;
		
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
		
	vector GetDirection()
	{
		return m_Direction;
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
	protected ref array<ref RayView> m_RayViews = {};
	
	void TranslationGizmo()
	{
		m_RayViews.Insert(new RayView(COLOR_RED, vector.Aside));
		m_RayViews.Insert(new RayView(COLOR_GREEN, vector.Up));
		m_RayViews.Insert(new RayView(COLOR_BLUE, vector.Forward));
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(OnUpdate);
	}
	
	void ~TranslationGizmo()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(OnUpdate);
		
		foreach (RayView view: m_RayViews) {
			delete view;
		}
		
		delete m_RayViews;
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
		
		
		Shape.CreateMatrix(gizmo_transform);
		
		ClearFlags(EntityFlags.VISIBLE, true);
		
		for (int i = 0; i < 3; i++) {
			RayView ray_view = m_RayViews[i];
			
			vector screen_origin = GetGame().GetScreenPos(gizmo_transform[3]);
			vector screen_end = GetGame().GetScreenPos(ray_view.GetDirection().Multiply4(gizmo_transform));
			
			if (screen_origin[2] < 0 || screen_end[2] < 0) {
				//return;
			}
			
			screen_origin[2] = 0;
			screen_end[2] = 0;
								
			float distance_to_target = vector.Distance(gizmo_transform[3], gizmo_transform[0].Multiply4(gizmo_transform[3]));								
			float x, y;
			ray_view.GetLayoutRoot().GetScreenSize(x, y);
			
			//float screen_size = 2 * Math.Tan(GetDayZGame().GetEditor().GetCamera().GetCurrentFOV() / 2) * gizmo_transform[0].Length();			
			
			vector twobytwo[3] = {
				Vector(screen_origin[0], screen_origin[1], 0),
				Vector(screen_end[0], screen_end[1], 0),
				Vector(0, 0, 0)
			};
						
			float angle = Math.Atan2(screen_end[1] - screen_origin[1], screen_end[0] - screen_origin[0]);
		
			vector matrix[4];
			Math3D.YawPitchRollMatrix(Vector(0, 0, angle * Math.RAD2DEG), matrix);
			ray_view.GetLayoutRoot().SetRotation(0, 0, angle * Math.RAD2DEG);
			
			vector huh = screen_origin.Multiply3(twobytwo);
			
			float height = 24;
			float width = (screen_end - screen_origin).Length();
			ray_view.GetLayoutRoot().SetSize(width, height);
			// - width / 2 + Math.Cos(angle) * width / 2
			//  - height / 2 + Math.Sin(angle) * height / 2
			ray_view.GetLayoutRoot().SetScreenPos(screen_origin[0] + Math.Cos(angle) * width / 2, screen_origin[1] + Math.Sin(angle) * height / 2);
		}
	}
}