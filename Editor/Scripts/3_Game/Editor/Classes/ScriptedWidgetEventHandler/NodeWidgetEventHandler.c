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
		if (Path == string.Empty) {
			delete this;
			return;
		}
		
		m_Node = NamedNode.Cast(DaysBefore.GetEditor().Get(Path));
		if (!m_Node) {
			delete this;
			return;
		}
		
		if (Button) {
			Button.SetHandler(this);
		}
		
		if (Icon) {
			Icon.LoadImageFile(0, m_Node.Icon.Regular());
			Icon.SetImage(0);
		}
		
		m_Node.State_OnChanged.Insert(OnStateChanged);
		m_Node.Interact_OnComplete.Insert(OnInteractComplete);
	}
	
	void OnStateChanged(NodeState node_state, bool state)
	{
	}
	
	void OnInteractComplete(NodeInteractType interact_type)
	{		
		if (interact_type & NodeInteractType.ENTER) {
			WidgetAnimator.AnimateColor(Icon, m_Node.Color.With(3, 255), 100);
			GetDayZGame().SetCursor(m_Node.Icon, m_Node.DisplayName);
			
			Icon.LoadImageFile(0, m_Node.Icon.Solid());
			Icon.SetImage(0);
		}
		
		if (interact_type & NodeInteractType.LEAVE) {
			WidgetAnimator.AnimateColor(Icon, LinearColor.WHITE.With(3, 100), 100);
			GetDayZGame().SetCursor();
			
			Icon.LoadImageFile(0, m_Node.Icon.Regular());
			Icon.SetImage(0);
		}
		
		if (interact_type & NodeInteractType.CONTEXT) {
			DaysBefore.GetEditor().GetHud().CreateContextMenu(m_Node);
		}
	}	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{		
		int interact_mask = m_Node.GetInteractMask();

		if (button == 0 && interact_mask & (NodeInteractType.ONCE | NodeInteractType.PRESS)) {
			m_Node.OnInteract(interact_mask & (NodeInteractType.ONCE | NodeInteractType.PRESS));
			return true;
		}
		
		if (button == 0 && interact_mask & NodeInteractType.HOLD) {
			m_Node.OnInteract(interact_mask & NodeInteractType.HOLD);
			return true;
		}
		
		if (button == 1 && interact_mask & NodeInteractType.CONTEXT) {
			m_Node.OnInteract(interact_mask & NodeInteractType.CONTEXT);
			return true;
		}
				
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		int interact_mask = m_Node.GetInteractMask();

		if (button == 0 && interact_mask & (NodeInteractType.PRESS | NodeInteractType.RELEASE)) {
			m_Node.OnInteract(interact_mask & (NodeInteractType.PRESS | NodeInteractType.RELEASE));
			return true;
		}
				
		return super.OnMouseButtonUp(w, x, y, button);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		int interact_mask = m_Node.GetInteractMask();
		if (button == 0 && interact_mask & (NodeInteractType.CLICK)) {
			m_Node.OnInteract(interact_mask & (NodeInteractType.CLICK));
			return true;
		}
		
		return super.OnClick(w, x, y, button);
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		int interact_mask = m_Node.GetInteractMask();

		if (button == 0 && interact_mask & (NodeInteractType.DOUBLE)) {
			m_Node.OnInteract(interact_mask & (NodeInteractType.DOUBLE));
			return true;
		}

		return super.OnDoubleClick(w, x, y, button);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		int interact_mask = m_Node.GetInteractMask();
		if (interact_mask & (NodeInteractType.DRAG_START)) {
			m_Node.OnInteract(interact_mask & (NodeInteractType.DRAG_START));
		}
		
		return super.OnDrag(w, x, y);
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{		
		int interact_mask = m_Node.GetInteractMask();
		if (interact_mask & (NodeInteractType.DRAG)) {
			m_Node.OnInteract(interact_mask & (NodeInteractType.DRAG));
		}
		
		return super.OnDragging(w, x, y, reciever);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		int interact_mask = m_Node.GetInteractMask();
		if (interact_mask & (NodeInteractType.DROP)) {
			m_Node.OnInteract(interact_mask & (NodeInteractType.DROP));
		}
		
		return super.OnDrop(w, x, y, reciever);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		int interact_mask = m_Node.GetInteractMask();

		if (interact_mask & (NodeInteractType.ENTER)) {
			m_Node.OnInteract(interact_mask & (NodeInteractType.ENTER));
			return true;
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		int interact_mask = m_Node.GetInteractMask();
		if (interact_mask & (NodeInteractType.LEAVE)) {
			m_Node.OnInteract(interact_mask & (NodeInteractType.LEAVE));
			return true;
		}
				
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
}