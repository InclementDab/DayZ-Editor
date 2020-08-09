

typedef ref array<ref EditorBrush> EditorBrushSet;
typedef ref array<ref EditorBrushSettings> EditorBrushSettingsSet;
// use FLAGS to define brush settings, radius, density etc...


// This is the data that will be loaded from XML
class EditorBrushSettings
{
	string Name;
	
	float MinRadius, MaxRadius;
	ref TStringArray PlaceableObjects = new TStringArray();

	bool InsertPlaceableObject(string object_name, float object_frequency)
	{
		string model_name = GetGame().GetModelName(object_name);
		if (model_name == "UNKNOWN_P3D_FILE") {
			Print(string.Format("%1 is not a valid Object Type!", object_name));
			return false;
		}

		for (int i = 0; i < object_frequency * 100; i++)
			PlaceableObjects.Insert(object_name);
		
		return true;
	}
	
}

class EditorBrush
{
	protected BrushBase m_BrushDecal;
	protected ref EditorBrushSettings m_BrushSettings;

	protected static float m_BrushRadius = 20;
	static void SetRadius(float radius) { m_BrushRadius = radius; }
	static float GetRadius() { return m_BrushRadius; }
	
	protected static float m_BrushDensity = 0.5;
	static void SetDensity(float density) { m_BrushDensity = density; }
	static float GetDensity() { return m_BrushDensity; }
	
	// Private members
	private vector m_LastMousePosition;
	
	protected EditorObjectManager m_ObjectManager;
	
	void EditorBrush(EditorBrushSettings settings)
	{
		Print("EditorBrush");
		m_BrushSettings = settings;
		m_BrushDecal = GetGame().CreateObject("BrushBase", vector.Zero);
		//m_BrushDecal.SetTexture();
		m_ObjectManager = GetEditor().GetObjectManager();
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
		
		Input input = GetGame().GetInput();

		vector transform[4] = {
			Vector(m_BrushRadius / 10, 0, 0),
			Vector(0, m_BrushRadius / 10, 0),
			Vector(0, 0, m_BrushRadius / 10),
			CurrentMousePosition
		};
		
		m_BrushDecal.SetTransform(transform);
		
		if (GetEditor().GetUIManager().IsCursorOverUI()) return;
		/*
		if (input.LocalPress("UAFire")) {
			
		}*/
		
		if (input.LocalValue("UAFire")) {
			
			DuringMouseDown(CurrentMousePosition);
		}
	}
	
	
	
	void DuringMouseDown(vector position) 
	{ 
		
		if (vector.Distance(m_LastMousePosition, position) < (m_BrushRadius * Math.RandomFloat(0.5, 1))) return;
		m_LastMousePosition = position;
		
		ref EditorObjectDataSet data_set = new EditorObjectDataSet();
		
		for (int i = 0; i < m_BrushDensity * 100; i++) {
			
			
			vector pos = position;
			pos[0] = pos[0] + Math.RandomFloat(-m_BrushRadius / Math.PI, m_BrushRadius / Math.PI);
			pos[2] = pos[2] + Math.RandomFloat(-m_BrushRadius / Math.PI, m_BrushRadius / Math.PI);
			
			string object_name = m_BrushSettings.PlaceableObjects.Get(Math.RandomInt(0, m_BrushSettings.PlaceableObjects.Count() - 1));
			Object placed_object = GetGame().CreateObjectEx(object_name, pos, ECE_NONE);
			
		
			vector clip_info[2];
			vector size;
			placed_object.ClippingInfo(clip_info);
			size[0] = Math.AbsFloat(clip_info[0][0]) + Math.AbsFloat(clip_info[1][0]);
			size[1] = Math.AbsFloat(clip_info[0][1]) + Math.AbsFloat(clip_info[1][1]);
			size[2] = Math.AbsFloat(clip_info[0][2]) + Math.AbsFloat(clip_info[1][2]);
			
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + size[1] / 2.4;
			
			vector direction = Math3D.GetRandomDir();
			direction[1] = Math.RandomFloat(-0.05, 0.05);
			placed_object.SetDirection(direction);
			
			
			data_set.Insert(new EditorObjectData(object_name, pos, placed_object.GetOrientation(), EditorObjectFlags.NONE));
			
			GetGame().ObjectDelete(placed_object);		
		}
		
		m_ObjectManager.CreateObjects(data_set);
	}
	
	void SetSettings(EditorBrushSettings settings) { m_BrushSettings = settings; }
	EditorBrushSettings GetSettings() { return m_BrushSettings; }
	
	string GetName() { return m_BrushSettings.Name; }
	
}



class DeleteBrush: EditorBrush
{	
	
	override void DuringMouseDown(vector position)
	{
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		vector contact_pos, contact_dir;
		int component;
		set<Object> results = new set<Object>();
		DayZPhysics.RaycastRV(position - surface_normal * 5, position + surface_normal * 500, contact_pos, contact_dir, component, results, null, null, false, false, 0, EditorBrush.GetRadius() / 2, CollisionFlags.ALLOBJECTS);
		
		foreach (Object r: results) {
					
			EditorObject eo = m_ObjectManager.GetEditorObject(r.GetID());
			if (eo != null) {
				EditorEvents.SelectObject(this, eo);
				m_ObjectManager.DeleteSelection();
			} else {
				GetGame().ObjectDelete(r);
			}
			
		}
		
		
		
		
	}

}
