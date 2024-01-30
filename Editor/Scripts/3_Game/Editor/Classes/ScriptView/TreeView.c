class TreeView: ScriptView
{
	protected TreeViewController m_TemplateController;
		
	TextWidget Text;
	
	Widget Panel, Children, Outline;
	ImageWidget IconImage, CollapseIcon, Texture;
	ButtonWidget Collapse;
	
	protected bool m_IsBeingDragged;

	protected TreeNode m_Node;

	void TreeView(TreeNode node)
	{		
		m_TemplateController = TreeViewController.Cast(m_Controller);
		m_Node = node;

		SetText(m_Node.GetDisplayName());
		
		IconImage.LoadImageFile(0, m_Node.GetIcon().Solid());
		IconImage.SetImage(0);
	}
			
	void OnSelectionChanged(bool state)
	{
		Panel.SetColor(EditorColors.SELECT * state);
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
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w) {
			case Collapse: {
				ShowChildren(!Children.IsVisible());
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
		Panel.SetAlpha(1.0);
		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		switch (w) {
			case Texture: {
				ShowChildren(false);
				return true;
			}
			
			case Panel: {
				switch (button) {
					case 0: {
						if (!KeyState(KeyCode.KC_LSHIFT)) {
							m_Node.GetEditor().ClearSelections();
						}
						
						if (KeyState(KeyCode.KC_LCONTROL)) {
							m_Node.GetEditor().ToggleSelect(m_Node);
						} else {
							m_Node.GetEditor().Select(m_Node);
						}
						
						return true;
					}
					
					case 1: {
						EditorHud hud = GetDayZGame().GetEditor().GetHud();
						hud.GetTemplateController().MenuItems.Clear();
						
						if (m_Node.CreateContextMenu(hud.GetTemplateController().MenuItems)) {
							hud.Menu.Show(true);
							hud.Menu.SetScreenPos(x, y);
						} else {
							hud.Menu.Show(false);
						}
						
						return true;
					}
				}
			}
		}	
		
		return false;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		Print("OnDoubleClick" + w);
		
		return super.OnDoubleClick(w, x, y, button);
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