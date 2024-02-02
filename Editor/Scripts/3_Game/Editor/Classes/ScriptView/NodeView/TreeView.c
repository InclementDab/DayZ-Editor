class TreeView: NodeView
{
	protected TreeViewController m_TemplateController;
		
	TextWidget Text;
	EditBoxWidget Edit;
	
	Widget Panel, Wrapper, Children, Outline, Texture, Collapse, Minimize, Dot;
	ImageWidget IconImage, CollapseIcon;
	ButtonWidget CollapseButton;
	
	protected bool m_IsBeingDragged;

	void TreeView(TreeNode node)
	{
		m_TemplateController = TreeViewController.Cast(m_Controller);		
		SetText(m_Node.GetDisplayName());
		
		IconImage.LoadImageFile(0, m_Node.GetIcon().Regular());
		IconImage.SetImage(0);
	}
			
	override void OnStateChanged(TreeNode node, TreeNodeState state)
	{
		super.OnStateChanged(node, state);
		
		EditorHud hud = m_Node.GetEditor().GetHud();
		Panel.SetAlpha(node.GetState().IsActive() || node.GetState().IsContext() || node.GetState().IsDragging());
		Outline.SetAlpha(node.GetState().IsHover());
		Dot.Show(node.GetState().IsFocus());
		
		if (!node.GetState().IsDragging()) {
			m_LayoutRoot.SetPos(0, 0);
		}
		
		/*
		if (state.IsActive()) {
			array<string> xor_selections = GetXorSelections();
			foreach (string xor: xor_selections) {
				TreeNode xor_node = Parent[xor];
				if (!xor_node) {
					Error(string.Format("[%1] couldnt find node to xor %2", m_UUID, xor));
					continue;
				}
				
				if (state ^ xor_node.GetState()) {
					//xor_node.RemoveState(TreeNodeState.ACTIVE);
				}
			}
		}*/
		
		if (state.IsHover()) {
			if (node.GetState().IsHover()) {
				hud.SetCursor(m_Node.GetIcon(), m_Node.GetDisplayName(), m_Node.GetUUID());	
			} else {
				hud.ClearCursor();
			}
		}
		
		if (state.IsContext()) {
			hud.GetTemplateController().MenuItems.Clear();
			if (node.GetState().IsContext() && m_Node.CreateContextMenu(hud.GetTemplateController().MenuItems)) {
				
				hud.Menu.Show(true);
				
				int screen_x, screen_y;
				GetScreenSize(screen_x, screen_y);
				
				int x, y;
				GetMousePos(x, y);
				
				float menu_w, menu_h;
				hud.Menu.GetScreenSize(menu_w, menu_h);
								
				x = Math.Min(x, screen_x - 15 - menu_w);
				x = Math.Max(15 + menu_w, x);
				
				y = Math.Min(y, screen_y - 15 - menu_h);
				y = Math.Max(15 + menu_h, y);
				
				hud.Menu.SetScreenPos(x, y);
			} else {
				hud.Menu.Show(false);
			}
		}
	}
	
	void AddView(notnull TreeView view)
	{
		m_TemplateController.ChildrenItems.Insert(view);
	}
	
	void SetText(string text)
	{
		Text.SetText(text);
		Edit.SetText(text);
				
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
	
	void EnableRename()
	{
		Text.SetAlpha(0.0);
		Edit.Show(true);
		SetFocus(Edit);
	}
		
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return super.OnClick(w, x, y, button);
		}
		
		switch (w) {
			case CollapseButton: {
				ShowChildren(!Children.IsVisible());
				return true;
			}
		}
				
		return super.OnClick(w, x, y, button);
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
		}
		
		return super.OnMouseEnter(w, x, y);
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
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		switch (w) {
			case Edit: {
				if (finished) {
					Text.SetText(Edit.GetText());
					Text.SetAlpha(1.0);
					Edit.Show(false);
					SetFocus(null);
					return true;
				}
				
				break;
			}
		}
		
		return false;
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