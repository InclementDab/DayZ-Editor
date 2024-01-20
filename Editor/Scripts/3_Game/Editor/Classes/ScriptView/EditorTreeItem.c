class EditorTreeItem: ScriptView
{
	protected EditorTreeItemController m_TemplateController;
	
	protected EditorTreeItem m_Parent;
	
	TextWidget Text;
	ImageWidget Icon, CollapseIcon, TreeDisplay;
	WrapSpacerWidget Children;
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
	
	void SetIcon(string icon)
	{
		Icon.Show(true);
		Icon.LoadImageFile(0, icon);
		Icon.SetImage(0);
	}
	
	void OnCollapseExecute(ButtonCommandArgs args)
	{
		ShowChildren(!Children.IsVisible());
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
		Children.Show(state);
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, "set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
		CollapseIcon.SetImage(0);
				
		float w, h;
		Children.GetSize(w, h);
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
			
	void OnSelectionChange(EditorSelectableBase selectable)
	{
		Button.SetColor(ARGB(255, 7, 111, 146) * selectable.IsSelected());
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
		return "Editor\\GUI\\layouts\\items\\EditorTreeItem.layout";
	}
}