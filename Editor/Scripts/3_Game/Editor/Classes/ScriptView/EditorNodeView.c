class EditorNodeView: ScriptView
{
	protected EditorNodeViewController m_TemplateController;
	
	protected EditorNodeView m_Parent;
	
	TextWidget Text;
	WrapSpacerWidget Children;
	Widget Panel, Spacer;
	ImageWidget IconImage, CollapseIcon;
		
	protected bool m_IsBeingDragged;
	protected string m_Text;
	
	protected EditorNode m_Selectable;
	
	void EditorNodeView(string text, EditorNode selectable, string icon)
	{
		m_TemplateController = EditorNodeViewController.Cast(m_Controller);
		m_Selectable = selectable;
		m_Selectable.OnSelectionChanged.Insert(OnSelectionChange);
		
		m_Text = text;
		SetText(m_Text);
		
		IconImage.LoadImageFile(0, icon);
		IconImage.SetImage(0);
	}
	
	void SetText(string text)
	{
		Text.SetText(text);
				
		float w, h;
		Text.GetScreenSize(w, h);
		Panel.SetScreenSize(w, h);
	}
		
	void Collapse(bool state)
	{
		Children.Show(!state);	
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state,"set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
		CollapseIcon.SetImage(0);
	}
	
	void OnCollapseExecute(ButtonCommandArgs args)
	{
		Collapse(Children.IsVisible());
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
					EditorNode.ClearSelections();
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
		Panel.SetColor(ARGB(255, 7, 111, 146) * selectable.IsSelected());
	}
					
	void SetParentTree(EditorNodeView parent)
	{
		m_Parent = parent;
		
		int i = 0;
		EditorNodeView upper_parent = parent;
		while (upper_parent) {
			i++;
			upper_parent = upper_parent.GetParent();
		}
		
		Spacer.SetSize(i * 14, 19.00);
	}
	
	EditorNodeView GetParentTree()
	{
		return m_Parent;
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