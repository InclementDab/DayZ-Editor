class EditorConfigPlaceableItem: EditorPlaceableObjectData
{
	protected string m_Path, m_Type;
	protected ref array<string> m_FullPath = {};
		
	void EditorConfigPlaceableItem(string path, string type)
	{
		m_Path = path;
		m_Type = type;
	}
	
	override string GetName()
	{
		return m_Type;
	}
	
	override EditorObjectDataCategory GetCategory()
	{
		GetGame().ConfigGetFullPath(string.Format("%1 %2", m_Path, m_Type), m_FullPath);
		
		if ((m_FullPath.Find("Weapon_Base") != -1) || (m_FullPath.Find("Inventory_Base")) != -1) {
			return EditorObjectDataCategory.DYNAMIC;
		} else if (m_FullPath.Find("HouseNoDestruct") != -1) {
			return EditorObjectDataCategory.STRUCTURE;
		} else if (m_FullPath.Find("Man") != -1) {
			return EditorObjectDataCategory.AI;
		} else if (m_FullPath.Find("Transport") != -1) {
			return EditorObjectDataCategory.VEHICLE;
		}
		
		return EditorObjectDataCategory.UNKNOWN;
	}
	
	override Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		Object object = GetGame().CreateObjectEx(m_Type, transform[3], ECE_LOCAL);								
		object.SetTransform(transform);
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
}