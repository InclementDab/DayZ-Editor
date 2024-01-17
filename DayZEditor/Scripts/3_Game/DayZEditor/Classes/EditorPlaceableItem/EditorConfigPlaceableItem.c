class EditorConfigPlaceableItem: EditorPlaceableItem
{
	protected string m_Path, m_Type;
	protected ref array<string> m_FullPath = {};
		
	void EditorConfigPlaceableItem(string path, string type)
	{
		m_Path = path;
		m_Type = type;
				
		m_DisplayName = type;
	}
	
	override EditorPlaceableItemCategory GetCategory()
	{
		GetGame().ConfigGetFullPath(string.Format("%1 %2", m_Path, m_Type), m_FullPath);
		
		if ((m_FullPath.Find("Weapon_Base") != -1) || (m_FullPath.Find("Inventory_Base")) != -1) {
			return EditorPlaceableItemCategory.DYNAMIC;
		} else if (m_FullPath.Find("HouseNoDestruct") != -1) {
			return EditorPlaceableItemCategory.STRUCTURE;
		} else if (m_FullPath.Find("Man") != -1) {
			return EditorPlaceableItemCategory.AI;
		} else if (m_FullPath.Find("Transport") != -1) {
			return EditorPlaceableItemCategory.VEHICLE;
		}
		
		return EditorPlaceableItemCategory.UNKNOWN;
	}
	
	override Object CreateObject(vector position, vector orientation, float scale, int flags = ECE_SETUP | ECE_LOCAL)
	{
		Object object = GetGame().CreateObjectEx(m_Type, position, flags);		
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
			
	override string GetModel()
	{
		string model;
		GetGame().ConfigGetText(string.Format("%1 %2 model", m_Path, m_Type), model);
		CF_File file = new CF_File(model);
		if (!file.IsValid()) {
			return string.Empty;
		}
		
		return file.GetFileName();
	}
	
	override EditorObjectData CreateData(vector position, vector orientation, float scale, int flags = EFE_DEFAULT)
	{
		return EditorObjectData.Create(m_Type, position, orientation, scale, flags);
	}
}