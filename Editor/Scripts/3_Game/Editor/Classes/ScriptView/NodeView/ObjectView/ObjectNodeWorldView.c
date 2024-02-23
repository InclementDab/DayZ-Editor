class ObjectNodeWorldView: NamedNodeView
{	
	protected ObjectNode m_ObjectNode;
	
	void ObjectNodeWorldView(Node node)
	{
		m_ObjectNode = ObjectNode.Cast(node);
	}
	
	override void OnInteract(NodeInteractType interact_type, Widget widget = null)
	{
		super.OnInteract(interact_type, widget);
		
		if (interact_type & NodeInteractType.ENTER) {
			WidgetAnimator.AnimateColor(Icon, EditorColor.SELECT, 100);
			WidgetAnimator.AnimateColor(IconOutline, m_ObjectNode.Color, 100);
		}
		
		if (interact_type & NodeInteractType.LEAVE && !m_ObjectNode.GetState().IsActive()) {
			WidgetAnimator.AnimateColor(Icon, LinearColor.Create(15, 15, 15), 100);
			WidgetAnimator.AnimateColor(IconOutline, LinearColor.WHITE, 100);
		}
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