class EditorStaticPlaceableItem: EditorPlaceable
{
	protected string m_Model;
	
	void EditorStaticPlaceableItem(string uuid, string display_name, string icon)
	{
		m_Model = uuid;
	}
	
	override Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		string model = m_Model;
		model.Replace("\\", "/");
		Object object = GetGame().CreateStaticObjectUsingP3D(model, transform[3], transform[2].VectorToAngles(), 1.0, true);
		object.SetTransform(transform);
		return object;
	}
}