class ObjectNodeWorldView: NamedNodeView
{	
	protected ObjectNode m_ObjectNode;
	
	void ObjectNodeWorldView(Node node)
	{
		m_ObjectNode = ObjectNode.Cast(node);
	}
	
	override void OnInteract(NodeInteractType interact_type)
	{
		super.OnInteract(interact_type);
		
		
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		vector base_position = m_ObjectNode.GetBasePoint();
			
		base_position = GetGame().GetScreenPos(m_ObjectNode.GetBasePoint());
		m_LayoutRoot.SetPos(base_position[0] - 12.5, base_position[1] - 12.5);
		m_LayoutRoot.Show(base_position[2] > 0 && !m_Node.GetState().IsSuppress());
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Marker.layout";
	}
}