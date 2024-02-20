class EditorButton: NodeWidgetEventHandler
{
	override void OnStateChanged(NodeState node_state, bool state)
	{		
		if (node_state.IsHover()) {
			if (state) {
				m_Node.GetEditor().GetHud().SetCursor(m_Node.GetIcon(), m_Node.GetDisplayName(), m_Node.GetShortcutString());
			} else {
				m_Node.GetEditor().GetHud().ClearCursor();
			}
		}
		
		if (Icon) {
			int color = ARGB(100 + node.GetState().IsHover() * 155.0, 255, 255, 255);
			if (node.GetState().IsActive()) {
				color = m_LayoutRoot.GetColor();
				Icon.LoadImageFile(0, node.GetIcon().Solid());
			} else {
				Icon.LoadImageFile(0, node.GetIcon().Regular());
			}
			
			Icon.SetColor(color);		
			Icon.SetImage(0);
		}
	}
}