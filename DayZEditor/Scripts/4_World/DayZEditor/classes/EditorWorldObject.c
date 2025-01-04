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
		if (type == string.Empty) {
			return null;
		}

		Object object;
		if (type.Contains(".p3d")) {
			string formatted_path = SystemPath.Format(type);
			formatted_path.Replace("\\", "/"); // static object creation likes this
			if (formatted_path[0] == "/") {
				formatted_path = formatted_path.Substring(1, formatted_path.Length() - 1);
			}
			
			object = GetGame().CreateStaticObjectUsingP3D(formatted_path, position, orientation, scale);
		}
		else
		{
			object = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_NOPERSISTENCY_CHAR | ECE_NOPERSISTENCY_WORLD);
		}

		if (!object)
		{
			EditorLog.Warning("EditorWorldObject: Invalid Object %1", type);
			return null;
		}

		object.SetOrientation(orientation);
		object.SetFlags(EntityFlags.VISIBLE, true);
		object.SetScale(scale);
		object.Update();
		
		// Needed for AI Placement			
		EntityAI entity_ai;
		if (Class.CastTo(entity_ai, object))
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(entity_ai.DisableSimulation, 15, 0, true);
			//entity_ai.DisableSimulation(true);

			// weeeeeeee
			if (GetEditor().GetSettings().SpawnItemsWithAttachments && (entity_ai.GetInventory().GetCargo() || entity_ai.GetInventory().GetAttachmentSlotsCount() > 0))
			{
				entity_ai.OnDebugSpawn();
			}
		}

		return object;
	}
}