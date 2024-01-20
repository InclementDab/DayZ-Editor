class EditorStaticPlaceableItem: EditorPlaceableObjectData
{
	protected string m_Model, m_Name;
	
	void EditorStaticPlaceableItem(string model)
	{
		m_Model = model;
		
		array<string> items = {};
		m_Model.Split("/", items);
		if (items.Count() != 0) {
			m_Name = items[items.Count() - 1];
		}	
	}
	
	override string GetName()
	{
		return m_Name;
	}
	
	override EditorObjectDataCategory GetCategory()
	{
		return EditorObjectDataCategory.STATIC;
	}
	
	override Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		Object object = GetGame().CreateStaticObjectUsingP3D(m_Model, transform[3], vector.Zero, 1.0);
		object.SetTransform(transform);
		return object;
	}
}