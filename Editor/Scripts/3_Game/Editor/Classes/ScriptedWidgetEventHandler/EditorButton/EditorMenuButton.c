class EditorMenuButton: EditorButton
{
	override void OnExecuted(TreeNode node, bool state)
	{
		EditorHud hud = GetDayZGame().GetEditor().GetHud();
	
		hud.Menu.Show(false);
		hud.GetTemplateController().MenuItems.Clear();
		if (!state) {
			return;
		}
		
		float x, y, w, h;
		m_LayoutRoot.GetScreenPos(x, y);
		m_LayoutRoot.GetScreenSize(w, h);
		hud.Menu.SetScreenPos(x, y + h);
		hud.Menu.Show(true);
		
		foreach (string uuid, TreeNode child: m_Node.Children) {
			CommandNode command_node = CommandNode.Cast(child);
			if (!command_node) {
				continue;
			}
			
			hud.GetTemplateController().MenuItems.Insert(new CommandMenuItem(command_node));
		}		
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Node) {
			GetDayZGame().GetEditor().GetHud().SetCursor(m_Node.GetIcon(), m_Node.GetDisplayName());
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{				
		if (!m_Button.GetState()) {
			WidgetAnimator.Animate(m_TextWidget, WidgetAnimatorProperty.COLOR_A, 200.0 / 255.0, 50);
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
}