class EditorStaticPlaceableItem: EditorPlaceableItem
{
	protected string m_Model;
	
	void EditorStaticPlaceableItem(string model)
	{
		m_Model = model;
	}
	
	override Object CreateObject(vector position, vector orientation, float scale)
	{
		return GetGame().CreateStaticObjectUsingP3D(m_Model, position, orientation, scale);
	}
	
	override string GetName()
	{
		array<string> items = {};
		m_Model.Split("/", items);
		if (items.Count() == 0) {
			return string.Empty;
		}
		
		return items[items.Count() - 1];
	}
	
	override EditorPlaceableItemCategory GetCategory()
	{
		return EditorPlaceableItemCategory.STATIC;
	}
	
	override EditorObjectData CreateData(vector position, vector orientation, float scale, int flags)
	{
		return EditorObjectData.Create(m_Model, position, orientation, scale, flags);
	}
}