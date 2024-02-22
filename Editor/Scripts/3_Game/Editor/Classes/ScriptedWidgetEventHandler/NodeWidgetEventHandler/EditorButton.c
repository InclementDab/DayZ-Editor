class EditorButton: NodeWidgetEventHandler
{
	override void OnStateChanged(NodeState node_state, bool state)
	{		
		if (node_state.IsHover()) {
			if (state) {
				//DaysBefore.GetEditor().GetHud().SetCursor(m_Node.Icon, m_Node.GetDisplayName(), m_Node.GetShortcutString());
			} else {
				//DaysBefore.GetEditor().GetHud().ClearCursor();
			}
		}
		
		if (Icon) {
			int color = ARGB(100 + node_state.IsHover() * 155.0, 255, 255, 255);
			if (state) {
				color = m_LayoutRoot.GetColor();
				//Icon.LoadImageFile(0, node.GetIcon().Solid());
			} else {
				//Icon.LoadImageFile(0, node.GetIcon().Regular());
			}
			
			Icon.SetColor(color);		
			Icon.SetImage(0);
		}
	}
}