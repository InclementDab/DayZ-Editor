class EditorNodeView: ScriptView
{
	protected EditorNodeViewController m_TemplateController;
	
	protected EditorNodeView m_Parent;
	
	TextWidget Text;
	
	Widget Panel;
	ImageWidget IconImage, CollapseIcon;
		
	protected bool m_IsBeingDragged;
	protected string m_Text;
	protected bool m_Children;
	
	protected EditorNode m_Node;
	
	void EditorNodeView(string text, EditorNode node, string icon)
	{
		m_TemplateController = EditorNodeViewController.Cast(m_Controller);
		m_Node = node;
		m_Node.OnSelectionChanged.Insert(OnSelectionChange);
		
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
		
	void ShowChildren(bool state)
	{
		m_TemplateController.CollapseState = state;
		m_TemplateController.NotifyPropertyChanged("CollapseState");
		
		// you only want to open upper containers when lower ones are opened. propagate up /\
		EditorNode parent = m_Node.GetParent();
		if (parent && state) {
			parent.GetNodeView().ShowChildren(true);
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
		Panel.SetColor(ARGB(255, 7, 111, 146) * selectable.IsSelected());
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