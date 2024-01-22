class EditorConfigPlaceable: EditorPlaceable
{
	protected string m_Type;
	
	void EditorConfigPlaceable(string uuid, string display_name, string icon)
	{
		m_Type = uuid;
	}
			
	override Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		Object object = GetGame().CreateObjectEx(m_Type, transform[3], ECE_LOCAL);								
		object.SetTransform(transform);
		object.Update();
		return object;
	}
}