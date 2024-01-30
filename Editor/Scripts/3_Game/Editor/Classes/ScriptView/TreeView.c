class TreeView: ScriptView
{
	protected TreeViewController m_TemplateController;
		
	TextWidget Text;
	
	Widget Panel, Children, Outline;
	ImageWidget IconImage, CollapseIcon;
	ButtonWidget Collapse;
	
	protected bool m_IsBeingDragged;

	protected TreeNode m_Node;

	void TreeView(TreeNode node)
	{
		m_TemplateController = TreeViewController.Cast(m_Controller);
		m_Node = node;

		SetText(m_Node.GetDisplayName());
		
		IconImage.LoadImageFile(0, m_Node.GetIcon().Regular());
		IconImage.SetImage(0);
	}
			
	void OnStateChanged(TreeNodeState state)
	{
		switch (state) {
			case TreeNodeState.EMPTY: {
				Outline.SetAlpha(0.0);
				Panel.SetAlpha(0);
				break;
			}
			
			case TreeNodeState.HOVER: {
				Outline.SetAlpha(1.0);
				break;
			}
			
			case TreeNodeState.ACTIVE: {
				Outline.SetAlpha(0.0);
				Panel.SetAlpha(1.0);
				break;
			}
		}
	}
	
	void SetText(string text)
	{
		Text.SetText(text);
				
		float w, h;
		Text.GetScreenSize(w, h);		
		Panel.SetScreenSize(w, h);
	}
		
	void ShowChildren(bool state)
	{
		if (!CollapseIcon) {
			return;
		}
		
		Children.Show(state);
		Children.Update(); //! importante
			
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, "set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
		CollapseIcon.SetImage(0);
		
		float w, h, x, y;
		Children.GetScreenSize(w, h);		
		m_LayoutRoot.GetScreenSize(x, y);
		m_LayoutRoot.SetScreenSize(x, h * state + 24);
		m_LayoutRoot.Update();
				
		// you only want to open upper containers when lower ones are opened. propagate up /\
		TreeNode parent = m_Node.GetParent();
		if (parent) {
			parent.GetNodeView().ShowChildren(true);
		}
	}
		
	void ApplyFilter(string filter)
	{
		filter.ToLower();
	
		string name = m_Node.GetDisplayName();
		name.ToLower();
	
		m_LayoutRoot.Show(name.Contains(filter));
	}
			
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (w) {
			case Panel: {
				switch (m_Node.GetInteractType()) {
					case TreeNodeInteract.HOLD: {
						m_Node.SetState(TreeNodeState.ACTIVE);
						return true;
					}
				}
				
				return false;
			}
		}
				
		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (w) {
			case Panel: {
				switch (m_Node.GetInteractType()) {
					case TreeNodeInteract.HOLD: {
						m_Node.SetState(TreeNodeState.EMPTY);
						return true;
					}
					
					case TreeNodeInteract.PRESS: {
						m_Node.SetState(TreeNodeState.ACTIVE);
						return true;
					}
				}
				
				return false;
			}
		}
				
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (w) {
			case Collapse: {
				ShowChildren(!Children.IsVisible());
				return true;
			}
			
			case Panel: {
				switch (m_Node.GetInteractType()) {
					case TreeNodeInteract.TOGGLE: {
						m_Node.ToggleState();
						return true;
					}
				}
				
				return false;
			}
		}
				
		return false;
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (w) {
			case Panel: {
				m_Node.ToggleState();
				return true;
			}
		}
		
		return false;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Node) {
			m_Node.SetState(TreeNodeState.HOVER);
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Node && m_Node.GetState() == TreeNodeState.HOVER) {
			m_Node.SetState(TreeNodeState.EMPTY);
			return true;
		}
		
		return false;
	}
			
	override bool OnDrag(Widget w, int x, int y)
	{		
		
		
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		Outline.SetAlpha(1.0);
			
		if (!RecursiveGetParent(reciever, "Root")) {
			return false;
		}
		
		// Warning:: unsafe
		TreeView bottom_view;
		reciever.GetUserData(bottom_view);
		if (!bottom_view) {
			return false;
		}
		
		//Print(bottom_view);
		
		return false;
	}
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		if (!RecursiveGetParent(reciever, "Root")) {
			return false;
		}
		
		// Warning:: unsafe
		TreeView bottom_view;
		reciever.GetUserData(bottom_view);
		if (!bottom_view) {
			return false;
		}
		
		//Print(bottom_view);
		//WidgetAnimator.Animate(bottom_view.Outline, WidgetAnimatorProperty.COLOR_A, 1.0, 0.0, 100);
		//bottom_view.Outline.SetAlpha(1.0);
		
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		Panel.SetPos(60, 0);
		Panel.Update();
		Panel.SetAlpha(0.0);
		return false;
	}
	
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		return true;
		if (!RecursiveGetParent(w, "Root")) {
			return false;
		}
		
		// Warning:: unsafe
		TreeView bottom_view;
		w.GetUserData(bottom_view);
		if (!bottom_view) {
			return false;
		}
		
		m_Node.Add(bottom_view.m_Node);
		
		return false;
	}
	
	TreeNode GetNode()
	{
		return m_Node;
	}
						
	TreeViewController GetTemplateController()
	{
		return m_TemplateController;
	}
		
	override typename GetControllerType()
	{
		return TreeViewController;
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\items\\TreeView.layout";
	}
}