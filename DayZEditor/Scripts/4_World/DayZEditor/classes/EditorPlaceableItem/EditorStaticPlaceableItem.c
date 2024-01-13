class EditorStaticPlaceableItem: EditorPlaceableItem
{
	protected ref CF_File m_Model;
	
	void EditorStaticPlaceableItem(notnull CF_File model)
	{
		m_Model = model;
	}
	
	override Object CreateObject(vector position, vector orientation, float scale)
	{		
		Object object = GetGame().CreateStaticObjectUsingP3D(m_Model.GetFullPath(), position, orientation, scale);								
		object.SetPosition(position);
		object.SetOrientation(orientation);
		object.SetScale(scale);
		object.Update();		
		return object;
	}
	
	override string GetName()
	{
		return m_Model.GetFileName();
	}
	
	override EditorPlaceableItemCategory GetCategory()
	{
		return EditorPlaceableItemCategory.STATIC;
	}
	
	override EditorObjectData CreateData(vector position, vector orientation, float scale, int flags)
	{
		return EditorObjectData.Create(m_Model.GetFullPath(), position, orientation, scale, flags);
	}
}