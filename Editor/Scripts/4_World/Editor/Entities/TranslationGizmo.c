class RayView: ScriptView
{
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
		m_RayViews.Insert(new RayView());
		
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
		
		vector transform[4];
		GetRenderTransform(transform);
		
		vector start_point = GetGame().GetScreenPos(transform[3]);
		vector end_point = GetGame().GetScreenPos(GetMemoryPointPos("x").Multiply4(transform));
		
		RayView x = m_RayViews[0];
		
		x.GetLayoutRoot().SetSize(Math.AbsFloat(end_point[0] - start_point[0]), 10);
		
		float theta = Math.Atan2(end_point[1], start_point[1]);
		
		x.GetLayoutRoot().SetPos(start_point[0], start_point[1]);
		
		vector camera_transform[3];
		GetDayZGame().GetEditor().GetCamera().GetTransform(camera_transform);
		
		vector inv_transform[3];
		Math3D.MatrixMultiply3(camera_transform, transform, inv_transform);
		
		vector angles = Math3D.MatrixToAngles(inv_transform);
		
		
		x.GetLayoutRoot().SetRotation(angles[0], angles[1], angles[2]);
	}
}