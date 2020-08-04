
static const ref array<string> FagusTreeStyle = { "bldr_plnt_t_FagusSylvatica_1f", "bldr_plnt_t_FagusSylvatica_1fb", "bldr_plnt_t_FagusSylvatica_1fc", "bldr_plnt_t_FagusSylvatica_1fd", "bldr_plnt_t_FagusSylvatica_1fe", "bldr_plnt_t_FagusSylvatica_1s", "bldr_plnt_t_FagusSylvatica_2d", "bldr_plnt_t_FagusSylvatica_2f", "bldr_plnt_c_ElytrigiaTall", "bldr_plnt_c_ElytrigiaForest", "bldr_plnt_c_Elytrigia", "bldr_plnt_b_PiceaAbies_1f", "bldr_plnt_b_BetulaPendula_1f", "bldr_plnt_b_FagusSylvatica_1f" };



static ref map<string, float> ChernarusTrees;

class EditorBrush
{
	protected EntityAI m_BrushDecal;
	protected float m_BrushRadius;
	
	void EditorBrush(float radius = 10)
	{
		Print("EditorBrush");
		m_BrushDecal = GetGame().CreateObject("BrushBase", vector.Zero);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(UpdateBrush);

		ChernarusTrees = new map<string, float>();
		ChernarusTrees.Insert("bldr_plnt_t_PiceaAbies_3f", 	1);
		ChernarusTrees.Insert("bldr_plnt_t_PiceaAbies_2sb", 1);
		ChernarusTrees.Insert("bldr_plnt_t_PiceaAbies_1f", 	1);
		ChernarusTrees.Insert("bldr_plnt_t_piceaabies_3d", 	0.2);
		ChernarusTrees.Insert("bldr_plnt_t_piceaabies_1s", 	0.2);
		ChernarusTrees.Insert("bldr_plnt_t_piceaabies_1fb", 0.1);
		ChernarusTrees.Insert("bldr_plnt_t_piceaabies_1f", 	0.1);
		ChernarusTrees.Insert("bldr_plnt_t_PiceaAbies_2s", 	0.1);
		ChernarusTrees.Insert("bldr_plnt_t_PiceaAbies_2s", 	0.1);
		
		SetRadius(radius);
	}
	
	void ~EditorBrush()
	{
		Print("~EditorBrush");
		GetGame().ObjectDelete(m_BrushDecal);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(UpdateBrush);
	}
	
	
	
	void UpdateBrush()
	{
		if (Editor.IsPlacing()) return;
		
		set<Object> o;
		vector CurrentMousePosition = MousePosToRay(o, null, EditorSettings.OBJECT_VIEW_DISTANCE, 0, true);
		m_BrushDecal.SetPosition(CurrentMousePosition);
		
		
		Input input = GetGame().GetInput();		
		if (input.LocalValue("UAFire")) {
			DuringMouseDown(CurrentMousePosition);
		}
	}
	
	void DuringMouseDown(vector position) { }
	
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


class NatureBrush: EditorBrush
{
	protected float m_BrushDensity;
	private vector m_LastMousePosition;
	
	private ref array<string> m_CurrentNatureData;
	
	void NatureBrush(float radius = 1076)
	{
		SetDensity(0.2);
		m_CurrentNatureData = new array<string>();
		foreach (string name, float rate: ChernarusTrees) {
		
			rate *= 100;
			for (int i = 0; i < rate; i++) {
				m_CurrentNatureData.Insert(name);
			}
			
		}
	}
	
	override void DuringMouseDown(vector position)
	{
		if (vector.Distance(m_LastMousePosition, position) < (m_BrushRadius * Math.RandomFloat(0.5, 1))) return;
		m_LastMousePosition = position;
		
		
		for (int i = 0; i < m_BrushDensity * 100; i++) {
			
			vector transform[4];
			Math3D.MatrixIdentity4(transform);
			transform[3] = position;
			
			vector pos = position;
			pos[0] = pos[0] + Math.RandomFloat(-m_BrushRadius / Math.PI, m_BrushRadius / Math.PI);
			pos[2] = pos[2] + Math.RandomFloat(-m_BrushRadius / Math.PI, m_BrushRadius / Math.PI);
	
			Object tree = GetGame().CreateObjectEx(m_CurrentNatureData.Get(Math.RandomInt(0, m_CurrentNatureData.Count() - 1)), pos, ECE_NONE);
			
			vector size = ObjectGetSize(tree);
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + size[1] / 2.4;
			tree.SetPosition(pos);
			
			vector direction = Math3D.GetRandomDir();
			direction[1] = Math.RandomFloat(-0.05, 0.05);
			tree.SetDirection(direction);
			
		}
	}
	
	void SetDensity(float density)
	{
		m_BrushDensity = density;
	}
}


class DeleteBrush: EditorBrush
{
	
	
	
	override void DuringMouseDown(vector position)
	{
		
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		vector contact_pos, contact_dir;
		int component;
		set<Object> results = new set<Object>();
		DayZPhysics.RaycastRV(position - surface_normal * 5, position + surface_normal * 500, contact_pos, contact_dir, component, results, null, null, false, false, 0, m_BrushRadius / 2);
		EditorUI.GetInstance().m_DebugText3.SetText(results.Count().ToString());
		
		foreach (Object r: results) {
			
			EditorObject eo = Editor.EditorObjectFromObject(r);
			if (eo != null) {
				Editor.DeleteObject(eo);
			} else {
				GetGame().ObjectDelete(r);
			}
		}
	}
}


class BoomBrush: EditorBrush
{
	
	override void DuringMouseDown(vector position)
	{
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		vector contact_pos, contact_dir;
		int component;
		set<Object> results = new set<Object>();
		DayZPhysics.RaycastRV(position, position + surface_normal * 50, contact_pos, contact_dir, component, results, null, null, false, false, 0, m_BrushRadius / 2, CollisionFlags.ALLOBJECTS);
		
		GetGame().CreateObject("ExplosionTest", position, true, false, false);
		
		
	}
}