class EditorMenuButton: EditorButton
{
	override void OnStateChanged(TreeNode node, TreeNodeState state)
	{
		EditorHud hud = GetDayZGame().GetEditor().GetHud();
		hud.Menu.Show(false);
		hud.GetTemplateController().MenuItems.Clear();
		
		float x, y, w, h;
		m_LayoutRoot.GetScreenPos(x, y);
		m_LayoutRoot.GetScreenSize(w, h);
		
		switch (state) {
			case TreeNodeState.EMPTY: {
				m_Node.GetEditor().GetHud().ClearCursor();
				WidgetAnimator.Animate(Text, WidgetAnimatorProperty.COLOR_A, 200.0 / 255.0, 50);
				break;
			}
			
			case TreeNodeState.HOVER: {
				m_Node.GetEditor().GetHud().SetCursor(m_Node.GetIcon(), m_Node.GetDisplayName());
				break;
			}
			
			case TreeNodeState.ACTIVE: {
				m_Node.GetEditor().GetHud().ClearCursor();
				hud.Menu.SetScreenPos(x, y + h);
				hud.Menu.Show(true);
				
				foreach (string uuid, TreeNode child: m_Node.Children) {
					CommandNode command_node = CommandNode.Cast(child);
					if (!command_node) {
						continue;
					}
					
					hud.GetTemplateController().MenuItems.Insert(new CommandMenuItem(command_node));
				}		
				
				break;
			}
		}		
	}
}