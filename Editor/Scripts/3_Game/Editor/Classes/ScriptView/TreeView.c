class TreeView: ScriptView
{
	static ref map<Widget, TreeView> AllTreeViews = new map<Widget, TreeView>();
	
	protected TreeViewController m_TemplateController;
		
	TextWidget Text;
	
	Widget Panel, Children;
	ImageWidget IconImage, CollapseIcon, Texture;
	ButtonWidget Collapse;
	
	protected bool m_IsBeingDragged;

	protected TreeNode m_Node;

	void TreeView(TreeNode node)
	{		
		m_TemplateController = TreeViewController.Cast(m_Controller);
		m_Node = node;
		
		m_Node.OnSelectionChanged.Insert(OnSelectionChange);
		
		SetText(m_Node.GetDisplayName());
		
		IconImage.LoadImageFile(0, m_Node.GetIcon().Solid());
		IconImage.SetImage(0);
		
		AllTreeViews[m_LayoutRoot] = this;
	}
	
	void ~TreeView()
	{
		AllTreeViews.Remove(m_LayoutRoot);
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
		
		Texture.Show(state);
		
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
			
	override void Update(float dt)
	{				
		if (m_IsBeingDragged) {
			
			if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
				m_IsBeingDragged = false;
				// drop!
			}
			
			int x, y;
			GetMousePos(x, y);
			
			Panel.SetPos(x, y);
			Text.SetPos(x, y);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w) {
			case Collapse: {
				ShowChildren(Collapse.GetState());
				return true;
			}
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == Texture || Collapse) {
			WidgetAnimator.Animate(Texture, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
			WidgetAnimator.Animate(Collapse, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
		}
		
		EditorHud hud = GetDayZGame().GetEditor().GetHud();
		
		hud.SetCursor(m_Node.GetIcon(), m_Node.GetDisplayName(), m_Node.GetUUID());
		
		PlaceableNode placeable = PlaceableNode.Cast(m_Node);
		if (placeable) {
			Object object = GetGame().CreateObjectEx(placeable.GetUUID(), vector.Zero, ECE_LOCAL);
			EntityAI entity = EntityAI.Cast(object);
			if (entity) {
				
				DayZPlayer player = DayZPlayer.Cast(entity);
				//hud.Player.Show(player != null);
				//hud.Item.Show(player == null);
				
				
				
				//hud.Tooltip.SetPos(30, y);
				//hud.Tooltip.Show(true);
			}
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{				
		if (w == Texture || Collapse) {
			WidgetAnimator.Animate(Texture, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
			WidgetAnimator.Animate(Collapse, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		}
		
		GetDayZGame().GetEditor().GetHud().ClearCursor();				
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (w) {
			case Texture: {
				ShowChildren(false);
				break;
			}
			
			case Panel: {
				switch (button) {
					case 0: {
						if (!KeyState(KeyCode.KC_LSHIFT)) {
							TreeNode.ClearSelections();
						}
						
						if (KeyState(KeyCode.KC_LCONTROL)) {
							m_Node.SetSelected(!m_Node.IsSelected());
						} else {
							m_Node.SetSelected(true);
						}
						
						return true;
					}
				}
				
				break;
			}
		}	
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		Print("OnDoubleClick" + w);
		
		return super.OnDoubleClick(w, x, y, button);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{		
		return super.OnDrag(w, x, y);
	}
			
	void OnSelectionChange(TreeNode selectable)
	{
		if (Panel) {
			Panel.SetColor(EditorColors.SELECT * selectable.IsSelected());
		}
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