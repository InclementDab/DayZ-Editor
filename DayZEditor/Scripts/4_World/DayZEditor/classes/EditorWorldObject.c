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
		Print(m_WorldObject);
		Print("HI");
		GetGame().ObjectDelete(m_WorldObject);	
		Print(m_WorldObject);
	}
	
	static Object CreateObject(string type, vector position = "0 0 0", vector orientation = "0 0 0", float scale = 1)	
	{
		type = type.Trim();
		type.Replace("\/", "\\");
		if (type == string.Empty) {
			return null;
		}
				
		Object object;
		if (type.Contains("\\") && ValidateObjectPath(type)) {
			object = GetGame().CreateStaticObjectUsingP3D(type, position, orientation, scale);
		} else {
			object = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_NOPERSISTENCY_CHAR | ECE_NOPERSISTENCY_WORLD);
		}
		
		if (!object) { 
			EditorLog.Warning("EditorWorldObject: Invalid Object %1", type);
			return null;
		}
		
		// Needed for AI Placement			
		EntityAI entity_ai;
		if (Class.CastTo(entity_ai, object)) {
			entity_ai.DisableSimulation(true);
						
			// weeeeeeee
			if (GetEditor().Settings.SpawnItemsWithAttachments && (entity_ai.GetInventory().GetCargo() || entity_ai.GetInventory().GetAttachmentSlotsCount() > 0)) {
				entity_ai.OnDebugSpawn();
			}
		}		
		
		object.SetOrientation(orientation);
		object.SetScale(scale);
		object.Update();		
		return object;
	}
	
	static bool ValidateObjectPath(string path)
	{
		foreach (string p: VALID_PATHS) {
			if (path.Contains(p)) {
				return true;
			}
		}
		
		return false;
	}
}