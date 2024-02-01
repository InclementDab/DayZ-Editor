class TreeView: ScriptView
{
	protected TreeViewController m_TemplateController;
		
	TextWidget Text;
	
	Widget Panel, Wrapper, Children, Outline, Texture, Collapse, Minimize, Dot;
	ImageWidget IconImage, CollapseIcon;
	ButtonWidget CollapseButton;
	
	protected bool m_IsBeingDragged;

	protected TreeNode m_Node;

	void TreeView(TreeNode node)
	{
		m_TemplateController = TreeViewController.Cast(m_Controller);
		m_Node = node;
		m_Node.State_OnChanged.Insert(OnStateChanged);
		
		SetText(m_Node.GetDisplayName());
		
		IconImage.LoadImageFile(0, m_Node.GetIcon().Regular());
		IconImage.SetImage(0);
	}
			
	void OnStateChanged(TreeNode node, TreeNodeState state)
	{
		Panel.SetAlpha(node.GetState().IsActive() || node.GetState().IsContext() || node.GetState().IsDragging());
		Outline.SetAlpha(node.GetState().IsHover());
		Dot.Show(node.GetState().IsFocus());
		
		if (!state.IsDragging()) {
			m_LayoutRoot.SetPos(0, 0);
		}
		
		m_Node.GetEditor().GetHud().SetCursor(m_Node.GetIcon(), m_Node.GetDisplayName(), m_Node.GetUUID());
	}
	
	void AddView(notnull TreeView view)
	{
		m_TemplateController.ChildrenItems.Insert(view);
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
		Children.Show(state);
		Children.Update(); //! importante
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, Symbols.CIRCLE_PLUS.Regular(), Symbols.CIRCLE_MINUS.Regular()));
		CollapseIcon.SetImage(0);
		Minimize.Show(state);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(RecalculateSize);
				
		// you only want to open upper containers when lower ones are opened. propagate up /\
		if (m_Node.Parent && m_Node.Parent.View) {
			m_Node.Parent.View.ShowChildren(true);
		}
	}
	
	void RecalculateSize()
	{
		float w, h, x, y;
		Children.Update();
		Children.GetScreenSize(w, h);	
		
		m_LayoutRoot.GetScreenSize(x, y);
		m_LayoutRoot.SetScreenSize(x, h * Children.IsVisible() + 30);
		m_LayoutRoot.Update();
		
		Collapse.Show(m_TemplateController.ChildrenItems.Count() > 0);
		Texture.Update();
	}
		
	// returns whether or not the filter was applied
	bool ApplyFilter(string filter)
	{		
		filter.ToLower();
		
		string name = m_Node.GetDisplayName();
		name.ToLower();
			
		string uuid = m_Node.GetUUID();
		uuid.ToLower();
		
		string search_bar = EditorHud.SEARCH_BAR_DEFAULT;
		search_bar.ToLower();
				
		bool applied = File.WildcardMatch(uuid, filter) || File.WildcardMatch(name, filter) || filter.Length() < 3;
		for (int i = 0; i < m_TemplateController.ChildrenItems.Count(); i++) {
			applied = applied || m_TemplateController.ChildrenItems[i].ApplyFilter(filter);
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(RecalculateSize);
		
		m_LayoutRoot.Show(applied);
		return applied;
	}
			
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{				
		if (button != 0) {
			return false;
		}
		
		switch (w) {
			case Minimize: {
				ShowChildren(false);
				return true;
			}
			
			case Wrapper: {
				switch (m_Node.GetInteractType()) {
					case TreeNodeInteract.HOLD:
					case TreeNodeInteract.PRESS: {
						m_Node.AddState(TreeNodeState.ACTIVE);
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
		if (button != 0) {
			return false;
		}
		
		switch (w) {
			case Wrapper: {
				switch (m_Node.GetInteractType()) {					
					case TreeNodeInteract.HOLD: {
						if (button == 0) {
							m_Node.RemoveState(TreeNodeState.ACTIVE);
							return true;
						}
						
						break;
					}
					
					case TreeNodeInteract.PRESS: {
						switch (button) {							
							case 1: {
								TreeNode.StateMachine.RemoveAllStates(TreeNodeState.CONTEXT);
								m_Node.AddState(TreeNodeState.CONTEXT);
								break;
							}
						}
						
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
		if (button != 0) {
			return false;
		}
		
		switch (w) {
			case CollapseButton: {
				ShowChildren(!Children.IsVisible());
				return true;
			}
			
			case Wrapper: {
				switch (m_Node.GetInteractType()) {
					case TreeNodeInteract.TOGGLE: {
						if (m_Node.HasState(TreeNodeState.ACTIVE)) {
							m_Node.RemoveState(TreeNodeState.ACTIVE);
						} else {
							m_Node.AddState(TreeNodeState.ACTIVE);
						}
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
		if (button != 0) {
			return false;
		}
		
		switch (w) {
			case Wrapper: {
				/*if (m_Node.HasState(TreeNodeState.ACTIVE)) {
					m_Node.RemoveState(TreeNodeState.ACTIVE);
				} else {
					m_Node.AddState(TreeNodeState.ACTIVE);
				}*/
				
				return false;
			}
		}
		
		return false;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case CollapseButton: {
				WidgetAnimator.AnimateColor(CollapseIcon, EditorColors.SELECT, 100);
				break;
			}
			
			case Minimize: {
				WidgetAnimator.Animate(Texture, WidgetAnimatorProperty.COLOR_A, 1.0, 100);
				break;
			}
			
			default: {
				m_Node.AddState(TreeNodeState.HOVER);
				break;
			}
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case CollapseButton: {
				WidgetAnimator.AnimateColor(CollapseIcon, ARGB(150, 255, 255, 255), 100);
				break;
			}
			
			case Minimize: {
				WidgetAnimator.Animate(Texture, WidgetAnimatorProperty.COLOR_A, 200.0 / 255.0, 100);
				break;
			}
			
			default: {
				m_Node.RemoveState(TreeNodeState.HOVER);
				break;
			}
		}
		
		return true;
	}
			
	override bool OnDrag(Widget w, int x, int y)
	{		
		m_Node.AddState(TreeNodeState.DRAGGING);
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{		
		return false;
	}
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{		
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		m_Node.RemoveState(TreeNodeState.ACTIVE);
		m_Node.RemoveState(TreeNodeState.DRAGGING);
		return false;
	}
	
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		if (!RecursiveGetParent(w, "Root")) {
			return false;
		}
		
		Class user_data;
		w.GetUserData(user_data);
		TreeView tree_view = TreeView.Cast(user_data);		
		if (!tree_view || tree_view == this) {
			return false;
		}
		
		TreeNode node = tree_view.GetNode();		
		delete tree_view;
		//GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(RecalculateSize);		
		m_Node.Add(node);
		return true;
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