class EditorNetworkedObject: EditorNode
{
	protected int m_Low, m_High;
	
	protected Object m_Object;
	
	void EditorNetworkedObject(string uuid, string display_name, string icon, notnull Object object)
	{
		object.GetNetworkID(m_Low, m_High);
		Print(m_Low);
		Print(m_High);
		
		
	}
	
	Object GetObject()
	{
		return m_Object;
	}
	
	override void Write(Serializer serializer, int version)
	{
		super.Write(serializer, version);
				
		serializer.Write(m_Low);
		serializer.Write(m_High);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		if (!super.Read(serializer, version)) {
			return false;
		}
				
		serializer.Read(m_Low);
		serializer.Read(m_High);
		
		return true;
	}
}