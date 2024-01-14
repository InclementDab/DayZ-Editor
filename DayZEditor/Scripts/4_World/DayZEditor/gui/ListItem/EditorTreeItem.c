class EditorTreeItem: ScriptView
{
	protected EditorTreeItemController m_TemplateController;
	
	protected EditorTreeItem m_Parent;
	
	TextWidget Text;
	ImageWidget Icon, CollapseIcon, ParentDisplay, TreeDisplay;
	WrapSpacerWidget Children;
	
	Widget Panel, CollapseWrapper, Spacer0;
	
	void EditorTreeItem()
	{
		m_TemplateController = EditorTreeItemController.Cast(m_Controller);
	}
	
	void OnCollapseExecute(ButtonCommandArgs args)
	{
		ShowChildren(!Children.IsVisible());
	}
	
	void ShowChildren(bool state)
	{
		Children.Show(state);
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, "set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
		CollapseIcon.SetImage(0);
		
		float w, h;
		m_LayoutRoot.GetScreenSize(w, h);
		
		m_LayoutRoot.SetScreenSize(w, state * 18 * (m_TemplateController.Children.Count() + 1));
	}
		
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (w) {
			case Panel: {
				if (button == 0) {
					SetFocus(w);
					return true;
				}
				
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
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