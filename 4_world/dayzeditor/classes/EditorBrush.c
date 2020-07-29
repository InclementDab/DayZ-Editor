
static const ref array<string> FagusTreeStyle = { "bldr_plnt_t_FagusSylvatica_1f", "bldr_plnt_t_FagusSylvatica_1fb", "bldr_plnt_t_FagusSylvatica_1fc", "bldr_plnt_t_FagusSylvatica_1fd", "bldr_plnt_t_FagusSylvatica_1fe", "bldr_plnt_t_FagusSylvatica_1s", "bldr_plnt_t_FagusSylvatica_2d", "bldr_plnt_t_FagusSylvatica_2f", "bldr_plnt_c_ElytrigiaTall", "bldr_plnt_c_ElytrigiaForest", "bldr_plnt_c_Elytrigia", "bldr_plnt_b_PiceaAbies_1f", "bldr_plnt_b_BetulaPendula_1f", "bldr_plnt_b_FagusSylvatica_1f" };

class EditorBrush
{
	protected Object m_BrushDecal;
	protected float m_BrushRadius = 10;
	
	void EditorBrush()
	{
		Print("EditorBrush");
		m_BrushDecal = GetGame().CreateObject("BrushBase", vector.Zero);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(UpdateBrush);
		SetRadius(10);
	}
	
	void ~EditorBrush()
	{
		Print("~EditorBrush");
		GetGame().ObjectDelete(m_BrushDecal);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(UpdateBrush);
	}
	
	
	private vector m_LastMousePosition;
	void UpdateBrush()
	{
		if (Editor.IsPlacing()) return;
		
		set<Object> o;
		vector CurrentMousePosition = MousePosToRay(o, null, EditorSettings.OBJECT_VIEW_DISTANCE, 0, true);
		m_BrushDecal.SetPosition(CurrentMousePosition);
		
		
		Input input = GetGame().GetInput();		
		if (input.LocalValue("UAFire") && vector.Distance(m_LastMousePosition, CurrentMousePosition) > (m_BrushRadius * Math.RandomFloat(0.5, 1.5))) {
			
			float density = 1;
			
			for (int i = 0; i < density * 100; i++) {
				
				
				Object t = GetGame().CreateObjectEx(FagusTreeStyle.Get(Math.RandomInt(0, FagusTreeStyle.Count() - 1)), CurrentMousePosition, ECE_NONE);
				vector size = ObjectGetSize(t);
				vector pos = CurrentMousePosition;
				pos[0] = pos[0] + Math.RandomFloat(-m_BrushRadius/2, m_BrushRadius/2);
				pos[2] = pos[2] + Math.RandomFloat(-m_BrushRadius/2, m_BrushRadius/2);
				pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + size[1] / 2;
				vector dir = Math3D.GetRandomDir();
				dir[1] = 0;
				t.SetDirection(dir);
				t.SetPosition(pos);
			}
			

			
			m_LastMousePosition = CurrentMousePosition;
		}
	}
	
	void SetRadius(float radius)
	{
		m_BrushRadius = radius;
		
		vector transform[4] = {
			Vector(radius / 10, 0, 0),
			Vector(0, radius / 10, 0),
			Vector(0, 0, radius / 10),
			m_BrushDecal.GetPosition()
		};
		m_BrushDecal.SetTransform(transform);
	}
	
	
}