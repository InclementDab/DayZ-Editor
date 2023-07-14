class EditorWorldObject
{
	static const ref array<string> VALID_PATHS = { 
		"DZ\\plants", 
		"DZ\\plants_bliss",
		"DZ\\rocks",
		"DZ\\rocks_bliss",
	};
	
	protected Object m_WorldObject;
	Object GetWorldObject() 
	{
		return m_WorldObject;
	}
	
	void ~EditorWorldObject()
	{
		GetGame().ObjectDelete(m_WorldObject);	
	}
	
	static Object CreateObject(string type, vector position = "0 0 0", vector orientation = "0 0 0", float scale = 1)	
	{
		type = type.Trim();
		type.Replace("\/", "\\");
		if (type == string.Empty) {
			return null;
		}
		
		Object object;
		if (type.Contains("\\")) {
			object = GetGame().CreateStaticObjectUsingP3D(type, position, orientation, scale);
		} else {
			object = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_NOPERSISTENCY_CHAR | ECE_NOPERSISTENCY_WORLD);
		}
		
		if (!object) { 
			EditorLog.Warning("EditorWorldObject: Invalid Object %1", type);
			return null;
		}
		
		
		EntityAI entity_ai;
		if (Class.CastTo(entity_ai, object)) {
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(entity_ai.DisableSimulation, 37, false, true);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(entity_ai.DisableSimulation, 37, false, false);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(entity_ai.DisableSimulation, 37, false, true);
		}
						
		object.SetPosition(position);
		object.SetOrientation(orientation);
		object.SetScale(scale);
		object.Update();		
		return object;
	}
}