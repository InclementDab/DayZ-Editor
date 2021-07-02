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
	
	static Object CreateObject(string type, vector position = "0 0 0", vector orientation = "0 0 0", float scale = 1, bool physics = true)
	{
		// Set to ECE_SETUP for AI compat. DONT ADD ECE_LOCAL
		type = type.Trim();
		if (type == string.Empty) return null;
		
		int flags = ECE_INITAI;
		if (physics) {
			flags |= ECE_CREATEPHYSICS;
		}
		
		Object obj;		
		if (!Class.CastTo(obj, GetGame().CreateObjectEx(type, position, flags))) {
			EditorLog.Error("EditorWorldObject: Invalid Object %1", type);
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