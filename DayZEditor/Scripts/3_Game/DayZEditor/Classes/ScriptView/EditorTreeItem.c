class EditorTreeItem: ScriptView
{
	protected EditorTreeItemController m_TemplateController;
	
	protected EditorTreeItem m_Parent;
	
	TextWidget Text;
	ImageWidget Icon, CollapseIcon, ParentDisplay, TreeDisplay;
	WrapSpacerWidget ChildrenWrapper;
	ButtonWidget Button;
	
	Widget CollapseWrapper, Spacer0, DragPanel;
	
	protected bool m_IsBeingDragged;
	
	protected EditorSelectableBase m_Selectable;
	
	void EditorTreeItem(string text, EditorSelectableBase selectable_base)
	{
		m_TemplateController = EditorTreeItemController.Cast(m_Controller);
		m_TemplateController.Text = text;
		m_TemplateController.NotifyPropertyChanged("Text");	
			
		m_Selectable = selectable_base;
		m_Selectable.OnSelectionChanged.Insert(OnSelectionChange);
				
		ShowChildren(false);
	}
	
	void OnCollapseExecute(ButtonCommandArgs args)
	{
		ShowChildren(!ChildrenWrapper.IsVisible());
	}
	
	void ApplyFilter(string filter)
	{
		Show(false);
		filter.ToLower();
		
		bool has_visible_children;
		for (int i = 0; i < m_TemplateController.Children.Count(); i++) {
			
			string name = m_TemplateController.Children[i].GetTemplateController().Text;
			name.ToLower();
			
			m_TemplateController.Children[i].Show(name.Contains(filter));
			if (name.Contains(filter)) {
				Show(true);
			}
		}
	}
	
	void ShowChildren(bool state)
	{
		ChildrenWrapper.Show(state);
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, "set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
		CollapseIcon.SetImage(0);
		
		float w, h;
		m_LayoutRoot.GetScreenSize(w, h);
		m_LayoutRoot.SetScreenSize(w, Ternary<float>.If(state, 18 * (m_TemplateController.Children.Count()) + 18, 18));
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
	
	override bool OnDrag(Widget w, int x, int y)
	{
		switch (w) {
			case DragPanel: {
				m_IsBeingDragged = true;
				return true;
			}
		}
		
		return super.OnDrag(w, x, y);
	}
	
	void OnExecute(ButtonCommandArgs args)
	{
		if (args.GetMouseButton() != 0) {
			return;
		}
		
		m_Selectable.SetSelected(true);
	}
		
	void OnSelectionChange(EditorSelectableBase selectable_base, bool state)
	{
		if (state) {
			WidgetAnimator.AnimateColor(Button, ARGB(255, 75, 119, 190), 50);			
		} else {
			WidgetAnimator.AnimateColor(Button, ARGB(0, 0, 0, 0), 50);
		}
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