class EditorNetworkedObject: EditorNode
{	
	void EditorNetworkedObject(string uuid, string display_name, string icon, notnull Object object)
	{
		int low, high;
		object.GetNetworkID(low, high);
		
		// Object is networked
		if (low != 0 || high != -1) {
			m_UUID = low.ToString() + high.ToString();
		}	
	}
}