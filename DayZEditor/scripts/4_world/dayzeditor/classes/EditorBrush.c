

typedef ref array<ref EditorBrush> EditorBrushSet;
typedef ref map<string, ref EditorBrushData> EditorBrushDataSet;

// This is the data that will be loaded from XML
class EditorBrushData
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
	protected EntityAI m_BrushDecal;
	protected ref EditorBrushData m_BrushData;

	protected static float m_BrushRadius = 20;
	static void SetRadius(float radius) { m_BrushRadius = radius; }
	static float GetRadius() { return m_BrushRadius; }
	
	protected static float m_BrushDensity = 0.5;
	static void SetDensity(float density) { m_BrushDensity = density; }
	static float GetDensity() { return m_BrushDensity; }
	
	// Private members
	private vector m_LastMousePosition;

	
	void EditorBrush(EditorBrushData settings)
	{
		EditorLog.Trace("EditorBrush");
		m_BrushData = settings;
		m_BrushDecal = GetGame().CreateObject("BrushBase", vector.Zero);	
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(UpdateBrush);
	}
	
	void ~EditorBrush()
	{
		EditorLog.Trace("~EditorBrush");
		GetGame().ObjectDelete(m_BrushDecal);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(UpdateBrush);
	}
	
	
	
	void SetBrushTexture(string texture)
	{
		EditorLog.Trace("EditorBrush::SetBrushTexture " + texture);
		m_BrushDecal.SetObjectTexture(0, texture);
		m_BrushDecal.Update();
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
		
		//if (GetEditor().GetUIManager().IsCursorOverUI()) return;
		/*
		if (input.LocalPress("UAFire")) {
			
		}*/
		
		if (input.LocalValue("UAFire")) {
			
			DuringMouseDown(CurrentMousePosition);
		}
	}
	
	
	
	void DuringMouseDown(vector position) 
	{ 
		//if (m_BrushSettings.PlaceableObjects.Count() == 0) return;
		if (vector.Distance(m_LastMousePosition, position) < (m_BrushRadius * Math.RandomFloat(0.5, 1))) return;
		m_LastMousePosition = position;
		
		ref EditorObjectDataSet data_set = new EditorObjectDataSet();
		
		for (int i = 0; i < m_BrushDensity * 100; i++) {
			
			
			vector pos = position;
			pos[0] = pos[0] + Math.RandomFloat(-m_BrushRadius / Math.PI, m_BrushRadius / Math.PI);
			pos[2] = pos[2] + Math.RandomFloat(-m_BrushRadius / Math.PI, m_BrushRadius / Math.PI);
			
			string object_name = m_BrushData.PlaceableObjects.Get(Math.RandomInt(0, m_BrushData.PlaceableObjects.Count() - 1));
						
			
			data_set.InsertEditorData(EditorObjectData.Create(object_name, pos, vector.Up, EditorObjectFlags.NONE));
			
			/*
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
			*/
			
		}
		
		GetEditor().CreateObjects(data_set);
		
	}
	
	void SetSettings(EditorBrushData settings) { m_BrushData = settings; }
	EditorBrushData GetSettings() { return m_BrushData; }
	
	string GetName() { return m_BrushData.Name; }
	
}



class DeleteBrush: EditorBrush
{	
	
	void DeleteBrush(EditorBrushData settings)
	{
		EditorLog.Trace("DeleteBrush");
		SetBrushTexture("DayZEditor\\Editor\\data\\BrushDelete.paa");
	}
	
	override void DuringMouseDown(vector position)
	{
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		vector contact_pos, contact_dir;
		int component;
		set<Object> results = new set<Object>();
		DayZPhysics.RaycastRV(position - surface_normal * 5, position + surface_normal * 500, contact_pos, contact_dir, component, results, null, null, false, false, 0, EditorBrush.GetRadius() / 2, CollisionFlags.ALLOBJECTS);
		GetEditor().ClearSelection();
		
		// todo this is deleting one at a time. use DeleteObjects smile :)
		foreach (Object r: results) {
					
			EditorObject eo = GetEditor().GetPlacedObjectById(r.GetID());
			if (eo != null) {
				GetEditor().DeleteObject(eo);
				
			} else {
				GetGame().ObjectDelete(r);
			}
		}
		
		
		
	}

}
