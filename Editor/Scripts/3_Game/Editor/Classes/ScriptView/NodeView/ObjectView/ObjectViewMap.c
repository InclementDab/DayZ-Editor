class ObjectViewMap: ObjectView
{
	override void Update(float dt)
	{				
		MapWidget map_widget = MapWidget.Cast(m_LayoutRoot.GetParent());
		if (!map_widget) {
			return;
		}
		
		vector base_point = map_widget.MapToScreen(m_ObjectNode.GetBasePoint());
		
		m_LayoutRoot.SetSort(999);
		m_LayoutRoot.SetScreenPos(base_point[0], base_point[1]);
		m_LayoutRoot.Update();
	}
}