class EditorConfigPlaceable: EditorPlaceable
{
	override Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		Object object = GetGame().CreateObjectEx(m_UUID, transform[3], ECE_LOCAL);								
		object.SetTransform(transform);
		object.Update();
		return object;
	}
}