class TreeView: NodeView
{
	protected TreeViewController m_TemplateController;
	protected NamedNode m_NamedNode;
	
	TextWidget Text;
	EditBoxWidget Edit;
	
	Widget Wrapper, Collapse, Children, Outline, Texture, Minimize, Hide, Icon;
	ImageWidget IconImage, CollapseIcon, HideIcon;
	ButtonWidget CollapseButton, HideButton;
	
	protected bool m_IsBeingDragged;

	void TreeView(Node node)
	{
		m_NamedNode = NamedNode.Cast(node);
		m_TemplateController = TreeViewController.Cast(m_Controller);		
		SetText(m_NamedNode.DisplayName);
		Hide.Show(m_NamedNode.GetState().IsSuppress());
		Collapse.Show(m_NamedNode.GetState().IsExtend());
		Icon.Show(m_NamedNode.GetState().IsExtend());
		IconImage.LoadImageFile(0, m_NamedNode.Icon.Regular());
		IconImage.SetImage(0);
		
		if (node.GetState().IsDrag()) {
			Wrapper.SetFlags(WidgetFlags.DRAGGABLE);
		} else {
			Wrapper.ClearFlags(WidgetFlags.DRAGGABLE);
		}
	}
			
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		EditorHud hud = m_Node.GetEditor().GetHud();
		Panel.SetAlpha(state || node_state.IsContext() || node_state.IsDrag());
		Outline.SetAlpha(node_state.IsHover());
		//if (node_state.IsFocus()) {
		//	Text.SetColor(EditorColor.FOCUS_1);
		//}
		
		if (!node_state.IsDrag()) {
			m_LayoutRoot.SetPos(0, 0);
		}
		
		if (node_state.IsSuppress()) {
			if (state) {
				HideIcon.LoadImageFile(0, Symbols.EYE_LOW_VISION.Regular());
			} else {
				HideIcon.LoadImageFile(0, Symbols.EYE.Regular());
			}
			
			HideIcon.SetImage(0);			
			float alpha = 1.0 - (node_state.IsSuppress() * 0.5);
			IconImage.SetAlpha(alpha);
			Text.SetAlpha(alpha);
			Edit.SetAlpha(alpha);
			CollapseIcon.SetAlpha(alpha);
			HideIcon.SetAlpha(alpha);
		}
		
		/*
		if (state.IsActive()) {
			array<string> xor_selections = GetXorSelections();
			foreach (string xor: xor_selections) {
				Node xor_node = Parent[xor];
				if (!xor_node) {
					Error(string.Format("[%1] couldnt find node to xor %2", m_UUID, xor));
					continue;
				}
				
				if (state ^ xor_node_state) {
					//xor_node.RemoveState(NodeState.ACTIVE);
				}
			}
		}*/
		
		if (node_state.IsHover()) {
			if (state) {
				hud.SetCursor(m_NamedNode.Icon, m_NamedNode.DisplayName, m_Node.GetUUID());	
			} else {
				hud.ClearCursor();
			}
		}
		
		if (node_state.IsContext()) {
			hud.GetTemplateController().MenuItems.Clear();
			if (state && m_Node.CreateContextMenu(hud.GetTemplateController().MenuItems)) {
				
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
		
		if (node_state.IsExtend()) {
			Children.Show(state);
			Children.Update(); //! importante
			
			Minimize.Show(state);
			
			if (state) {
				IconImage.LoadImageFile(0, m_NamedNode.Icon.Solid());		
			} else {
				IconImage.LoadImageFile(0, m_NamedNode.Icon.Regular());		
			}
			
			IconImage.SetImage(0);
			
			CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, Symbols.SQUARE_PLUS.Thin(), Symbols.SQUARE_MINUS.Thin()));
			CollapseIcon.SetImage(0);
			
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(RecalculateSize);
						
			// you only want to open upper containers when lower ones are opened. propagate up /\
			if (m_Node.GetParent()) {
				foreach (NodeView view: m_Node.GetParent().Views) {
					view.GetNode().AddState(NodeState.EXTEND);
				}
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
			
	void RecalculateSize()
	{
		float w, h, x, y;
		Children.Update();
		Children.GetScreenSize(w, h);	
		
		m_LayoutRoot.GetScreenSize(x, y);
		m_LayoutRoot.SetScreenSize(x, h * Children.IsVisible() + 30);
		m_LayoutRoot.Update();
		
		//Collapse.Show(m_TemplateController.ChildrenItems.Count() > 0);
		
		Minimize.SetScreenSize(8.0, h * Children.IsVisible());
		Minimize.Show(Children.IsVisible());
		Texture.Update();
	}
		
	// returns whether or not the filter was applied
	bool ApplyFilter(string filter)
	{		
		filter.ToLower();
		
		string name = m_NamedNode.DisplayName;
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
				if (m_Node.HasState(NodeState.EXTEND)) {
					m_Node.RemoveState(NodeState.EXTEND);
				} else {
					m_Node.AddState(NodeState.EXTEND);
				}

				return true;
			}
			
			case HideButton: {
				if (m_Node.HasState(NodeState.SUPPRESS)) {
					m_Node.RemoveState(NodeState.SUPPRESS);
				} else {
					m_Node.AddState(NodeState.SUPPRESS);
				}
				
				return true;
			}
		}
				
		return super.OnClick(w, x, y, button);
	}
			
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case CollapseButton: {
				WidgetAnimator.AnimateColor(CollapseIcon, EditorColor.SELECT, 100);
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