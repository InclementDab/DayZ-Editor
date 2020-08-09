

class EditorObjectNew
{
	protected Object 		m_WorldObject;
	Object GetObject() { return m_WorldObject; }
	
	void EditorObjectNew(string type_name, vector position, EditorObjectFlags flags)
	{
		EditorPrint("EditorObjectNew");
		m_WorldObject = GetGame().CreateObjectEx(type_name, position, ECE_NONE);
		//m_WorldObject.SetFlags(EntityFlags.STATIC, 1);
		//m_WorldObject.Update();	
	}
	
	
	
}