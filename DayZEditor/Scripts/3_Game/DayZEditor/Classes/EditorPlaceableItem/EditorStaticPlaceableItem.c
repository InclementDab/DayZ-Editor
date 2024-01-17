class EditorStaticPlaceableItem: EditorPlaceableItem
{
	protected string m_Model;
	
	void EditorStaticPlaceableItem(string model)
	{
		m_Model = model;
		
		array<string> items = {};
		m_Model.Split("/", items);
		if (items.Count() != 0) {
			m_DisplayName = items[items.Count() - 1];
		}	
	}
	
	override EditorPlaceableItemCategory GetCategory()
	{
		return EditorPlaceableItemCategory.STATIC;
	}
	
	override Object CreateObject(vector position, vector orientation, float scale, int flags = ECE_SETUP | ECE_LOCAL)
	{
		return GetGame().CreateStaticObjectUsingP3D(m_Model, position, orientation, scale);
	}
		
	override EditorObjectData CreateData(vector position, vector orientation, float scale, int flags = EFE_DEFAULT)
	{
		return EditorObjectData.Create(m_Model, position, orientation, scale, flags);
	}
}