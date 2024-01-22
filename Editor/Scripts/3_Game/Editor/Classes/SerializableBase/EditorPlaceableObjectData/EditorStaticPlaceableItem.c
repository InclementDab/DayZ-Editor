class EditorStaticPlaceableItem: EditorPlaceable
{
	protected string m_Model;
	
	void EditorStaticPlaceableItem(string uuid, string display_name, string icon)
	{
		m_Model = uuid;
		m_Model.Replace("\\", "/");
	}
	
	override Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		Print(m_Model);
		Object object = GetGame().CreateStaticObjectUsingP3D(m_Model, transform[3], transform[2].VectorToAngles(), 1.0);
		object.SetTransform(transform);
		return object;
	}
}