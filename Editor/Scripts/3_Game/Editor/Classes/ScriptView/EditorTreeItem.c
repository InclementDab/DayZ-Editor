class EditorTreeItem: ScriptView
{
	protected EditorTreeItemController m_TemplateController;
	
	protected EditorTreeItem m_Parent;
	
	TextWidget Text;
	WrapSpacerWidget Children;
	Widget Panel, Spacer;
		
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
	}
	
	void SetText(string text)
	{
		Text.SetText(text);
				
		float w, h;
		Text.GetScreenSize(w, h);
		Panel.SetScreenSize(w, h);
	}
	
	void SetIcon(string icon)
	{
		m_TemplateController.IconImage = icon;
		m_TemplateController.NotifyPropertyChanged("IconImage");
	}
	
	void OnCollapseExecute(ButtonCommandArgs args)
	{
		Children.Show(!Children.IsVisible());	
				
		m_TemplateController.CollapseIcon = Ternary<string>.If(!Children.IsVisible(),"set:dayz_gui image:Expand", "set:dayz_gui image:Collapse");
		m_TemplateController.NotifyPropertyChanged("CollapseIcon");
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
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w != Panel) {
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
		Panel.SetColor(ARGB(255, 7, 111, 146) * selectable.IsSelected());
	}
					
	void SetParentTree(EditorTreeItem parent)
	{
		m_Parent = parent;
		
		int i = 0;
		EditorTreeItem upper_parent = parent;
		while (upper_parent) {
			i++;
			upper_parent = upper_parent.GetParent();
		}
		
		Spacer.SetSize(i * 14, 19.00);
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