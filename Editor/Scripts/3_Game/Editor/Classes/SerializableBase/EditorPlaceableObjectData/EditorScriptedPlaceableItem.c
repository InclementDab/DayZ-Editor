class EditorScriptedPlaceableItem: EditorPlaceable
{
	protected typename m_Type;
		
	void EditorScriptedPlaceableItem(string uuid, string display_name, typename type)
	{
		m_Type = type;
	}
			
	override Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		Object object = GetGame().CreateObjectEx(m_Type.ToString(), transform[3], flags);				
		object.SetTransform(transform);
		object.Update();		
		return object;
	}
}