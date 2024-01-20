class EditorScriptedPlaceableItem: EditorPlaceableObjectData
{
	protected typename m_Type;
		
	void EditorScriptedPlaceableItem(typename type)
	{
		m_Type = type;
	}
	
	override string GetName()
	{
		return m_Type.ToString();
	}
		
	override Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{		
		Object object = GetGame().CreateObjectEx(m_Type.ToString(), transform[3], flags);		
		EntityAI entity_ai;
		if (Class.CastTo(entity_ai, object)) {
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(entity_ai.DisableSimulation, 37, false, true);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(entity_ai.DisableSimulation, 37, false, false);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(entity_ai.DisableSimulation, 37, false, true);
		}
						
		object.SetTransform(transform);
		object.Update();		
		return object;
	}
	
	override EditorObjectDataCategory GetCategory()
	{
		return EditorObjectDataCategory.SCRIPTED;
	}
}