class EditorMenuTab: ScriptedWidgetEventHandler
{
	reference string Menu;
	
	protected Widget m_LayoutRoot;
	protected ButtonWidget m_Button;
	protected TextWidget m_TextWidget;
	
	protected TreeNode m_Node;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	
		m_Button = FindWidget<ButtonWidget>.SearchDown(m_LayoutRoot, "Button");
		if (!m_Button) {
			Error("Button widget not found below " + m_LayoutRoot.GetName());
			return;
		}
		
		m_Button.SetHandler(this);
		
		m_TextWidget = FindWidget<TextWidget>.SearchDown(m_LayoutRoot, "Text");
		if (!m_TextWidget) {
			Error("Text widget not found below " + m_LayoutRoot.GetName());
			return;
		}
		
#ifdef WORKBENCH
		if (!GetDayZGame()) {
			return;
		}
#endif
		
		if (Menu != string.Empty) {	
			m_Node = GetDayZGame().GetEditor().GetMenu(Menu);
			if (m_Node) {
				//m_Node.OnSelectionChanged.Insert(OnExecuted);				
				m_Button.SetState(m_Node.GetDefaultState());
				m_TextWidget.SetText(m_Node.GetDisplayName());
			}
		}
	}
	
	void OnExecuted(TreeNode node, bool state)
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
		
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{		
		if (button == 0 && m_Node) {
			//m_Node.SetSelected(!m_Node.IsSelected());
		}
		
		return true;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.Animate(m_TextWidget, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
		
		if (m_Node) {
			GetDayZGame().GetEditor().GetHud().SetCursor(m_Node.GetIcon(), m_Node.GetDisplayName());
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{				
		GetDayZGame().GetEditor().GetHud().ClearCursor();
		
		if (!m_Button.GetState()) {
			WidgetAnimator.Animate(m_TextWidget, WidgetAnimatorProperty.COLOR_A, 200.0 / 255.0, 50);
		}
		
		return true;
	}
}