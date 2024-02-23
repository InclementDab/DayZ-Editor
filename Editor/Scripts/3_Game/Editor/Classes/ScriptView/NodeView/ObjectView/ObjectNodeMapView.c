class ObjectNodeMapView: NamedNodeMapView
{
	protected ObjectNode m_ObjectNode;
	
	void ObjectNodeMapView(Node node, MapView map_view)
	{
		m_ObjectNode = ObjectNode.Cast(node);
	}
	
	override void Update(float dt)
	{			
		super.Update(dt);
				
		vector base_point = m_Map.Map.MapToScreen(m_ObjectNode.GetBasePoint());
		
		m_LayoutRoot.SetSort(999);
		m_LayoutRoot.SetScreenPos(base_point[0], base_point[1]);
		m_LayoutRoot.Update();
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Marker.layout";
	}
}