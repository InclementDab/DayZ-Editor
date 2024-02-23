class EditorMenuButton: NodeWidgetEventHandler
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		EditorHud hud = DaysBefore.GetEditor().GetHud();
		hud.Menu.Show(false);
		hud.GetTemplateController().MenuItems.Clear();
		
		float x, y, w, h;
		m_LayoutRoot.GetScreenPos(x, y);
		m_LayoutRoot.GetScreenSize(w, h);
	
		GetDayZGame().SetCursor();
		if (state) {
				if (state) {
				hud.Menu.SetScreenPos(x, y + h);
				hud.Menu.Show(true);
				
				foreach (string uuid, Node child: m_Node.Children) {
					CommandNode command_node = CommandNode.Cast(child);
					if (!command_node) {
						continue;
					}
					
					hud.GetTemplateController().MenuItems.Insert(new MenuNode(command_node));
				}		
			} else {
				WidgetAnimator.Animate(Text, WidgetAnimatorProperty.COLOR_A, 200.0 / 255.0, 50);
			}
		}
		
		if (node_state.IsHover() && state) {
			GetDayZGame().SetCursor(m_Node.Icon, m_Node.DisplayName);
		}
	}
}