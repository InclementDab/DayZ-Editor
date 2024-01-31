class Base: ScriptView
{
	
}

class TreeView: ScriptView
{
	protected TreeViewController m_TemplateController;
		
	TextWidget Text;
	
	Widget Panel, Children, Outline, Texture, Collapse;
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
		Panel.SetAlpha(node.GetState().IsActive());
		Outline.SetAlpha(node.GetState().IsHover());
	}
	
	void AddView(notnull TreeView view)
	{
		m_TemplateController.ChildrenItems.Insert(view);
		
		// Recalculate size
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
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, Symbols.CIRCLE_PLUS.Regular(), Symbols.CIRCLE_MINUS.Regular()));
		CollapseIcon.SetImage(0);
		
		float w, h, x, y;
		Children.GetScreenSize(w, h);		
		m_LayoutRoot.GetScreenSize(x, y);
		m_LayoutRoot.SetScreenSize(x, h * state + 30);
		m_LayoutRoot.Update();
		
		Texture.Show(state);
				
		// you only want to open upper containers when lower ones are opened. propagate up /\
		TreeNode parent = m_Node.GetParent();
		if (parent && parent.View) {
			parent.View.ShowChildren(true);
		}
	}
		
	// returns whether or not the filter was applied
	bool ApplyFilter(string filter)
	{
		filter.ToLower();
	
		string name = m_Node.GetDisplayName();
		name.ToLower();
			
		string uuid = m_Node.GetUUID();
		uuid.ToLower();
		
		bool applied = name.Contains(filter) || uuid.Contains(filter) || filter.Contains(name) || filter.Contains(uuid);
		for (int i = 0; i < m_TemplateController.ChildrenItems.Count(); i++) {
			applied = applied || m_TemplateController.ChildrenItems[i].ApplyFilter(filter);
		}
		
		m_LayoutRoot.Show(applied);
		return applied;
	}
			
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{		
		if (button == 1) {
			m_Node.AddState(TreeNodeState.CONTEXT);
			return true;
		}
		
		if (button != 0) {
			return false;
		}
		
		switch (w) {
			case Texture: {
				ShowChildren(false);
				return true;
			}
			
			case Panel: {
				switch (m_Node.GetInteractType()) {
					case TreeNodeInteract.HOLD: {
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
			case Panel: {
				switch (m_Node.GetInteractType()) {
					case TreeNodeInteract.HOLD: {
						m_Node.RemoveState(TreeNodeState.ACTIVE);
						return true;
					}
					
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
			
			case Panel: {
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
			case Panel: {
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
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case CollapseButton: {
				WidgetAnimator.AnimateColor(CollapseIcon, EditorColors.SELECT, 100);
				break;
			}
			
			case Texture: {
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
			
			case Texture: {
				WidgetAnimator.Animate(Texture, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 100);
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
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		Outline.SetAlpha(1.0);
			
		//if (!RecursiveGetParent(reciever, "Root")) {
		//	return false;
		//}
		
		// Warning:: unsafe
		
		/*TreeView bottom_view;
		reciever.GetUserData(bottom_view);
		if (!bottom_view) {
			return false;
		}*/
		
		//Print(bottom_view);
		
		return false;
	}
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		//if (!RecursiveGetParent(reciever, "Root")) {
		//	return false;
		//}
		
		// Warning:: unsafe
		/*TreeView bottom_view;
		reciever.GetUserData(bottom_view);
		if (!bottom_view) {
			return false;
		}
		*/
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
		/*
		// Warning:: unsafe
		TreeView bottom_view;
		w.GetUserData(bottom_view);
		if (!bottom_view) {
			return false;
		}
		
		m_Node.Add(bottom_view.m_Node);
		*/
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