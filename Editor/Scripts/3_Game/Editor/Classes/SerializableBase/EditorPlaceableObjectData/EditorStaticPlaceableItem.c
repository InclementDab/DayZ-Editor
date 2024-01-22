class EditorStaticPlaceableItem: EditorPlaceable
{
	protected string m_Model;
	
	void EditorStaticPlaceableItem(string uuid, string display_name, string icon)
	{
		m_Model = uuid;
	}
	
	override Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		Object object = GetGame().CreateStaticObjectUsingP3D(m_Model, transform[3], vector.Zero, 1.0);
		object.SetTransform(transform);
		return object;
	}
}