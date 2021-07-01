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
		// Set to ECE_SETUP for AI compat. DONT ADD ECE_LOCAL
		type = type.Trim();
		if (type == string.Empty) return null;
		
		Object obj;		
		if (!Class.CastTo(obj, GetGame().CreateObjectEx(type, position, ECE_CREATEPHYSICS | ECE_INITAI))) {
			EditorLog.Error("EditorWorldObject: Invalid Object %1", type);
			return null;
		}

		// Needed for AI Placement			
		if (EntityAI.Cast(obj)) {
			EntityAI.Cast(obj).DisableSimulation(true);
		}
		 
		obj.SetAllowDamage(false);
		obj.SetOrientation(orientation);
		obj.SetScale(scale);
		obj.Update();
		
		return obj;
	}
}