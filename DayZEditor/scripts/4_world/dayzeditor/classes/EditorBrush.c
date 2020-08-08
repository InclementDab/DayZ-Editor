

static ref map<string, float> ChernarusTrees;
static ref map<string, float> ChernarusGrass;

class EditorBrush
{
	protected EntityAI m_BrushDecal;
	protected float m_BrushRadius;
	
	void EditorBrush(float radius = 10)
	{
		Print("EditorBrush");
		EditorSettings.BRUSH_RADIUS = radius;
		m_BrushDecal = EntityAI.Cast(GetGame().CreateObject("BrushBase", vector.Zero));
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(UpdateBrush);
		
	}
	
	void ~EditorBrush()
	{
		Print("~EditorBrush");
		GetGame().ObjectDelete(m_BrushDecal);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(UpdateBrush);
	}
	
	
	
	void UpdateBrush()
	{
		if (GetEditor().IsPlacing()) return;
		
		set<Object> o;
		vector CurrentMousePosition = MousePosToRay(o, null, EditorSettings.OBJECT_VIEW_DISTANCE, 0, true);
		m_BrushDecal.SetPosition(CurrentMousePosition);
		
		
		Input input = GetGame().GetInput();		
		
		vector transform[4] = {
			Vector(EditorSettings.BRUSH_RADIUS / 10, 0, 0),
			Vector(0, EditorSettings.BRUSH_RADIUS / 10, 0),
			Vector(0, 0, EditorSettings.BRUSH_RADIUS / 10),
			m_BrushDecal.GetPosition()
		};
		
		m_BrushDecal.SetTransform(transform);
		
		if (input.LocalValue("UAFire") && !GetEditor().GetUIManager().IsCursorOverUI()) {
			DuringMouseDown(CurrentMousePosition);
		}
	}
	
	void DuringMouseDown(vector position) { }
}


class TreeBrush: EditorBrush
{
	
	private vector m_LastMousePosition;
	private ref array<string> m_CurrentNatureData;
	
	void TreeBrush(float radius = 10)
	{
		EditorSettings.BRUSH_DENSITY = 0.2;
		m_CurrentNatureData = new array<string>();
		
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

		
		foreach (string name, float rate: ChernarusTrees) {
		
			rate *= 100;
			for (int i = 0; i < rate; i++) {
				m_CurrentNatureData.Insert(name);
			}
			
		}
	}
	
	override void DuringMouseDown(vector position)
	{
		if (vector.Distance(m_LastMousePosition, position) < (EditorSettings.BRUSH_RADIUS * Math.RandomFloat(0.5, 1))) return;
		m_LastMousePosition = position;
		
		for (int i = 0; i < EditorSettings.BRUSH_DENSITY * 100; i++) {
						
			vector pos = position;
			pos[0] = pos[0] + Math.RandomFloat(-EditorSettings.BRUSH_RADIUS / Math.PI, EditorSettings.BRUSH_RADIUS / Math.PI);
			pos[2] = pos[2] + Math.RandomFloat(-EditorSettings.BRUSH_RADIUS / Math.PI, EditorSettings.BRUSH_RADIUS / Math.PI);
	
			Object tree = GetGame().CreateObjectEx(m_CurrentNatureData.Get(Math.RandomInt(0, m_CurrentNatureData.Count() - 1)), pos, ECE_NONE);
			
			// remove this once we change Object to a lower abstracted version of EditorObject
			vector clip_info[2];
			vector size;	
			tree.ClippingInfo(clip_info);
			size[0] = Math.AbsFloat(clip_info[0][0]) + Math.AbsFloat(clip_info[1][0]);
			size[1] = Math.AbsFloat(clip_info[0][1]) + Math.AbsFloat(clip_info[1][1]);
			size[2] = Math.AbsFloat(clip_info[0][2]) + Math.AbsFloat(clip_info[1][2]);
			
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + size[1] / 2.4;
			tree.SetPosition(pos);
			
			vector direction = Math3D.GetRandomDir();
			direction[1] = Math.RandomFloat(-0.05, 0.05);
			tree.SetDirection(direction);
			
		}
	}
}


class GrassBrush: EditorBrush
{
	
	private vector m_LastMousePosition;
	private ref array<string> m_CurrentNatureData;
	
	void GrassBrush(float radius = 10)
	{
		EditorSettings.BRUSH_RADIUS = radius;
		m_CurrentNatureData = new array<string>();

		ChernarusGrass = new map<string, float>();
		ChernarusGrass.Insert("bldr_plnt_c_grassdry2_summer",     1);
		ChernarusGrass.Insert("bldr_plnt_c_grassdry3_summer",     0.2);
		ChernarusGrass.Insert("bldr_plnt_c_grassdrytall2_summer", 0.2);
		ChernarusGrass.Insert("bldr_plnt_c_grassdrytall3_summer", 0.1);
		ChernarusGrass.Insert("bldr_plnt_c_grassdrytall_summer",  0.1);
		ChernarusGrass.Insert("bldr_plnt_c_grassdry_summer",      1);
		
		foreach (string name, float rate: ChernarusGrass) {
		
			rate *= 100;
			for (int i = 0; i < rate; i++) {
				m_CurrentNatureData.Insert(name);
			}
			
		}
	}
	
	override void DuringMouseDown(vector position)
	{
		if (vector.Distance(m_LastMousePosition, position) < (EditorSettings.BRUSH_RADIUS * Math.RandomFloat(0.5, 0.8))) return;
		m_LastMousePosition = position;
		
		for (int i = 0; i < EditorSettings.BRUSH_DENSITY * 100; i++) {
						
			vector pos = position;
			pos[0] = pos[0] + Math.RandomFloat(-EditorSettings.BRUSH_RADIUS / Math.PI, EditorSettings.BRUSH_RADIUS / Math.PI);
			pos[2] = pos[2] + Math.RandomFloat(-EditorSettings.BRUSH_RADIUS / Math.PI, EditorSettings.BRUSH_RADIUS / Math.PI);
	
			Object tree = GetGame().CreateObjectEx(m_CurrentNatureData.Get(Math.RandomInt(0, m_CurrentNatureData.Count() - 1)), pos, ECE_NONE);
			
			// remove this once we change Object to a lower abstracted version of EditorObject
			vector clip_info[2];
			vector size;	
			tree.ClippingInfo(clip_info);
			size[0] = Math.AbsFloat(clip_info[0][0]) + Math.AbsFloat(clip_info[1][0]);
			size[1] = Math.AbsFloat(clip_info[0][1]) + Math.AbsFloat(clip_info[1][1]);
			size[2] = Math.AbsFloat(clip_info[0][2]) + Math.AbsFloat(clip_info[1][2]);
			
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + size[1] / 2.4;
			tree.SetPosition(pos);
			
			vector direction = Math3D.GetRandomDir();
			direction[1] = Math.RandomFloat(-0.05, 0.05);
			tree.SetDirection(direction);
			
		}
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
		DayZPhysics.RaycastRV(position - surface_normal * 5, position + surface_normal * 500, contact_pos, contact_dir, component, results, null, null, false, false, 0, m_BrushRadius / 2, CollisionFlags.ALLOBJECTS);
		
		foreach (Object r: results) {
			
			EditorObject eo = GetEditor().GetObjectManager().GetEditorObject(r);
			if (eo != null) {
				GetEditor().GetObjectManager().SelectObject(eo);
				GetEditor().GetObjectManager().DeleteSelection();
			} else {
				GetGame().ObjectDelete(r);
			}
		}
	}
}

class BoomBrush: EditorBrush
{
	private vector m_LastMousePosition;
	
	void BoomBrush(float radius = 10)
	{
		EditorSettings.BRUSH_RADIUS = radius;
		EditorSettings.BRUSH_DENSITY = 0.2;
	}
	
	override void DuringMouseDown(vector position)
	{
		if (vector.Distance(m_LastMousePosition, position) < (EditorSettings.BRUSH_RADIUS * Math.RandomFloat(0.5, 1))) return;
		m_LastMousePosition = position;
		
		for (int i = 0; i < (EditorSettings.BRUSH_RADIUS * 10) + 1; i++) {
			vector pos = position;
			pos[0] = pos[0] + Math.RandomFloat(-EditorSettings.BRUSH_RADIUS / Math.PI, EditorSettings.BRUSH_RADIUS / Math.PI);
			pos[2] = pos[2] + Math.RandomFloat(-EditorSettings.BRUSH_RADIUS / Math.PI, EditorSettings.BRUSH_RADIUS / Math.PI);
	
			GetGame().CreateObject("ExplosionTest", pos);
		}
	}
}