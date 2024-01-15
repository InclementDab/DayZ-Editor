class EditorTreeItem: ScriptView
{
	protected EditorTreeItemController m_TemplateController;
	
	protected EditorTreeItem m_Parent;
	
	TextWidget Text;
	ImageWidget Icon, CollapseIcon, ParentDisplay, TreeDisplay;
	WrapSpacerWidget Children;
	
	Widget Panel, CollapseWrapper, Spacer0, DragPanel;
	
	protected bool m_IsBeingDragged;
	
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
		
		m_LayoutRoot.SetScreenSize(w, !state * 18 + state * (m_TemplateController.Children.Count() + 1));
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

	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		switch (w) {
			case DragPanel: {
				//float f_x, f_y;
				//DragPanel.SetPos(x, y);
				//Text.SetPos(x, y);
				return true;
			}
		}
		
		return super.OnDragging(w, x, y, reciever);
	}	
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		switch (w) {
			case DragPanel: {
				//float f_x, f_y;
				//DragPanel.SetPos(x, y);
				//Text.SetPos(x, y);
				return true;
			}
		}
		
		return super.OnDraggingOver(w, x, y, reciever);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		switch (w) {
			case DragPanel: {
				//float f_x, f_y;
				///Panel.GetPos(f_x, f_y);
				//DragPanel.SetPos(f_x, f_y);
				//DragPanel.Show(false);
				
				//Text.SetPos(f_x, f_y);
				break;
			}
		}
		
		return super.OnDrop(w, x, y, reciever);
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		switch (w) {
			case Panel: {
				if (button == 0) {
					//SetFocus(w);
					return true;
				}
				
				break;
			}
		}
		
		return super.OnMouseButtonUp(w, x, y, button);
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