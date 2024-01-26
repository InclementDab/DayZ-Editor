class EditorNodeView: ScriptView
{
	protected EditorNodeViewController m_TemplateController;
	
	protected EditorNodeView m_Parent;
	
	TextWidget Text;
	
	Widget Panel, Children;
	ImageWidget Collapse, IconImage, CollapseIcon, Texture;
	
	protected bool m_ChildrenVisible;
	protected bool m_IsBeingDragged;
	protected string m_Text;
	
	protected Symbols m_Icon;
	
	protected EditorNode m_Node;

	void EditorNodeView(string text, EditorNode node, Symbols icon)
	{
		m_TemplateController = EditorNodeViewController.Cast(m_Controller);
		m_Node = node;
		m_Icon = icon;
		m_Node.OnSelectionChanged.Insert(OnSelectionChange);
		
		m_Text = text;
		SetText(m_Text);
		
		IconImage.LoadImageFile(0, m_Icon.Solid());
		IconImage.SetImage(0);
	}
	
	void SetText(string text)
	{
		Text.SetText(text);
				
		float w, h;
		Text.GetScreenSize(w, h);		
		Panel.SetScreenSize(w, h);
	}
	
	void ShowChildren(bool state, float offset = 0)
	{
		m_ChildrenVisible = state;
		if (!CollapseIcon) {
			Print(m_Node.GetDisplayName());
			return;
		}
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, "set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
		CollapseIcon.SetImage(0);
		
		float w, h;
		Children.Show(state);
		Children.GetScreenSize(w, h);
		//Print(h);
		float x, y;
		m_LayoutRoot.GetSize(x, y);
		offset += (h * state);
		m_LayoutRoot.SetScreenSize(x, 18 + offset);
		m_LayoutRoot.Update();
		
		// you only want to open upper containers when lower ones are opened. propagate up /\
		EditorNode parent = m_Node.GetParent();
		if (parent) {
			parent.GetNodeView().ShowChildren(true, offset);
		}
	}
		
	void ApplyFilter(string filter)
	{
		filter.ToLower();
	
		string name = m_Text;
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
				ShowChildren(!m_ChildrenVisible);
				return true;
			}
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == Texture) {
			WidgetAnimator.Animate(Texture, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
		}
		
		EditorHud hud = GetDayZGame().GetEditor().GetHud();
		
		hud.SetCursor(new EditorTooltip(m_Icon, m_Node.GetUUID(), m_Node.GetUUID()));
		
		EditorPlaceable placeable = EditorPlaceable.Cast(m_Node);
		if (placeable) {
			Object object = GetGame().CreateObjectEx(placeable.GetUUID(), vector.Zero, ECE_LOCAL);
			EntityAI entity = EntityAI.Cast(object);
			if (entity) {
				
				DayZPlayer player = DayZPlayer.Cast(entity);
				hud.Player.Show(player != null);
				hud.Item.Show(player == null);
				
				if (player) {
					hud.Player.SetPlayer(player);
				} else {
					hud.Item.SetItem(entity);
					hud.Item.SetView(entity.GetViewIndex());
				}
				
				hud.Tooltip.SetPos(30, y);
				hud.Tooltip.Show(true);
			}
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{				
		if (w == Texture) {
			WidgetAnimator.Animate(Texture, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		}
		
		EditorHud hud = GetDayZGame().GetEditor().GetHud();
		
		hud.ClearCursor();
		hud.Tooltip.Show(false);
				
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w != Panel) {
			return super.OnMouseButtonDown(w, x, y, button);
		}
		
		switch (button) {
			case 0: {
				if (!KeyState(KeyCode.KC_LSHIFT)) {
					EditorNode.ClearSelections();
				}
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					m_Node.SetSelected(!m_Node.IsSelected());
				} else {
					m_Node.SetSelected(true);
				}
				
				return true;
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
			
	void OnSelectionChange(EditorNode selectable)
	{
		Panel.SetColor(EditorColors.SELECT * selectable.IsSelected());
	}
						
	EditorNodeViewController GetTemplateController()
	{
		return m_TemplateController;
	}
		
	override typename GetControllerType()
	{
		return EditorNodeViewController;
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\items\\EditorNodeView.layout";
	}
}