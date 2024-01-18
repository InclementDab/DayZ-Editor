class EditorTreeItem: ScriptView
{
	protected EditorTreeItemController m_TemplateController;
	
	protected EditorTreeItem m_Parent;
	
	TextWidget Text;
	ImageWidget Icon, CollapseIcon, TreeDisplay;
	WrapSpacerWidget ChildrenWrapper;
	ButtonWidget Button;
	
	Widget CollapseWrapper, WrapPadding;
	
	protected bool m_IsBeingDragged;
	protected string m_Text;
	
	protected EditorSelectableBase m_Selectable;
	
	void EditorTreeItem(string text, EditorSelectableBase selectable)
	{
		m_TemplateController = EditorTreeItemController.Cast(m_Controller);
		m_Selectable = selectable;
		m_Selectable.OnSelectionChanged.Insert(OnSelectionChange);
		
		m_Text = text;
		SetText(m_Text);
		
		ShowChildren(false);
	}
	
	void SetText(string text)
	{
		Text.SetText(text);
				
		float w, h;
		Text.GetScreenSize(w, h);
		Button.SetScreenSize(w, h);
	}
	
	void OnCollapseExecute(ButtonCommandArgs args)
	{
		ShowChildren(!ChildrenWrapper.IsVisible());
	}
	
	void ApplyFilter(string filter)
	{
		filter.ToLower();
	
		string name = m_Text;
		name.ToLower();
	
		m_LayoutRoot.Show(name.Contains(filter));
	}
	
	void ShowChildren(bool state)
	{
		ChildrenWrapper.Show(state);
		WrapPadding.Show(state);
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, "set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
		CollapseIcon.SetImage(0);
				
		float w, h;
		ChildrenWrapper.GetSize(w, h);
		WrapPadding.SetScreenSize(18, h);
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
			
			Button.SetPos(x, y);
			Text.SetPos(x, y);
		}
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w != Button) {
			return super.OnMouseButtonDown(w, x, y, button);
		}
		
		switch (button) {
			case 0: {
				if (!KeyState(KeyCode.KC_LSHIFT)) {
					EditorSelectableBase.ClearSelections();
				}
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					m_Selectable.SetSelected(!m_Selectable.IsSelected());
				} else {
					m_Selectable.SetSelected(true);
				}
				
				return true;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{		
		return super.OnDrag(w, x, y);
	}
			
	void OnSelectionChange(EditorSelectableBase selectable, bool state)
	{
		Button.SetColor(ARGB(255, 75, 119, 190) * state);
	}
		
	bool IsBlacklistedItem(string item_type)
	{
		array<string> blacklist = { "DZ_LightAI", "Man", "Car" };
		foreach (string blacklist_check: blacklist) {
			if (GetGame().IsKindOf(item_type, blacklist_check)) {
				return true;
			}
		}
		
		return false;
	}
			
	void SetParentTree(EditorTreeItem parent)
	{
		m_Parent = parent;
	}
	
	EditorTreeItem GetParentTree()
	{
		return m_Parent;
	}
	
	EditorTreeItemController GetTemplateController()
	{
		return m_TemplateController;
	}
		
	override typename GetControllerType()
	{
		return EditorTreeItemController;
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\items\\EditorTreeItem.layout";
	}
}