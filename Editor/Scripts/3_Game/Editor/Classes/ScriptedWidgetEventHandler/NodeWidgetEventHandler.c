class NodeWidgetEventHandler: ScriptedWidgetEventHandler
{
	reference string Path;
	protected NamedNode m_Node;
	
	protected Widget m_LayoutRoot;
	
	TextWidget Text;
	ButtonWidget Button;
	ImageWidget Icon;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	
#ifdef COMPONENT_SYSTEM
		if (!GetDayZGame()) {
			return;
		}
#endif
		
		Button = FindWidget<ButtonWidget>.SearchDown(m_LayoutRoot, "Button");				
		Icon = FindWidget<ImageWidget>.SearchDown(m_LayoutRoot, "Icon");		
		Text = FindWidget<TextWidget>.SearchDown(m_LayoutRoot, "Text");		
		if (Path != string.Empty) {
			m_Node = NamedNode.Cast(DaysBefore.Root[Path]);
			if (m_Node) {
				if (Button) {
					Button.SetHandler(this);
				}
				
				if (Icon) {
					Icon.LoadImageFile(0, m_Node.Icon.Regular());
					Icon.SetImage(0);
				}
				
				m_Node.State_OnChanged.Insert(OnStateChanged);
				OnStateChanged(m_Node.GetDefaultState(), true);
			}
		}
	}
	
	void OnStateChanged(NodeState node_state, bool state)
	{
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (m_Node.GetInteractType()) {
			case SandboxNodeInteract.HOLD: {
				m_Node.AddState(NodeState.ACTIVE);
				return true;
			}
		}
				
		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (!m_Node) {
			return false;
		}
		
		if (button == 1) {
			m_Node.AddState(NodeState.CONTEXT);
			return true;
		}
		
		if (button == 0) {
			switch (m_Node.GetInteractType()) {
				case SandboxNodeInteract.HOLD: {
					m_Node.RemoveState(NodeState.ACTIVE);
					return true;
				}
				
				case SandboxNodeInteract.ONCE:
				case SandboxNodeInteract.PRESS: {
					m_Node.AddState(NodeState.ACTIVE);									
					return true;
				}
			}
		}
				
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (m_Node.GetInteractType()) {
			case SandboxNodeInteract.TOGGLE: {
				if (m_Node.HasState(NodeState.ACTIVE)) {
					m_Node.RemoveState(NodeState.ACTIVE);
				} else {
					m_Node.AddState(NodeState.ACTIVE);
				}
				return true;
			}
		}
				
		return false;
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (m_Node.GetInteractType()) {
			case SandboxNodeInteract.DOUBLE: {
				if (m_Node.HasState(NodeState.ACTIVE)) {
					m_Node.RemoveState(NodeState.ACTIVE);
				} else {
					m_Node.AddState(NodeState.ACTIVE);
				}
				return true;
			}
		}
		
		return false;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Node) {
			m_Node.AddState(NodeState.HOVER);
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Node) {
			m_Node.RemoveState(NodeState.HOVER);
		}
				
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
}