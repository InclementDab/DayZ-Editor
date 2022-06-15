class EditorWorldObject
{	
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
		if (type == string.Empty) {
			return null;
		}
		
		Object obj;		
		if (!Class.CastTo(obj, GetGame().CreateObjectEx(type, position, ECE_CREATEPHYSICS | ECE_INITAI))) { 
			EditorLog.Warning("EditorWorldObject: Invalid Object %1", type);
			return null;
		}
		
		// Needed for AI Placement			
		EntityAI entity_ai;
		if (Class.CastTo(entity_ai, obj)) {
			entity_ai.DisableSimulation(true);
						
			// weeeeeeee
			if (GetEditor().Settings.SpawnItemsWithAttachments && (entity_ai.GetInventory().GetCargo() || entity_ai.GetInventory().GetAttachmentSlotsCount() > 0)) {
				entity_ai.OnDebugSpawn();
			}
		}		
		
		obj.SetOrientation(orientation);
		obj.SetScale(scale);
		obj.Update();		
		return obj;
	}
}