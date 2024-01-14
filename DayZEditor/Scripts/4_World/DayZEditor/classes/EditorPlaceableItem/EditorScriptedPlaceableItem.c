class EditorScriptedPlaceableItem: EditorPlaceableItem
{
	protected typename m_Type;
		
	void EditorScriptedPlaceableItem(typename type)
	{
		m_Type = type;
	}
		
	override Object CreateObject(vector position, vector orientation, float scale)
	{		
		Object object = GetGame().CreateObjectEx(m_Type.ToString(), position, ECE_SETUP);		
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
	
	override string GetName()
	{
		return m_Type.ToString();
	}
	
	override EditorPlaceableItemCategory GetCategory()
	{
		return EditorPlaceableItemCategory.SCRIPTED;
	}
		
	override EditorObjectData CreateData(vector position, vector orientation, float scale, int flags)
	{
		return EditorObjectData.Create(m_Type.ToString(), position, orientation, scale, flags);
	}
}