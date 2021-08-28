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
		if (type == string.Empty) return null;
		
		Object obj;		
		// 2048 == ECE_INITAI
		if (!Class.CastTo(obj, GetGame().CreateObjectEx(type, position, ECE_CREATEPHYSICS | 2048))) { 
			EditorLog.Warning("EditorWorldObject: Invalid Object %1", type);
			return null;
		}

		// Needed for AI Placement			
		if (EntityAI.Cast(obj)) {
			EntityAI.Cast(obj).DisableSimulation(true);
		}
		
		obj.SetOrientation(orientation);
		obj.SetScale(scale);
		obj.Update();
		
		return obj;
	}
}