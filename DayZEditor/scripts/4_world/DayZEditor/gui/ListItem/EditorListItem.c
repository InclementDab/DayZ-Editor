


class EditorListItem: ScriptViewTemplate<EditorListItemController>
{
	protected int m_NestIndex;
		
	static int COLOR_ON_SELECTED = COLOR_BLUE;
	static int COLOR_ON_DESELECTED = ARGB(140,5,5,5);
	static int COLOR_ON_HOVER = COLOR_SALMON;
	
	protected Widget EditorListItemButtonFrame;
	protected Widget EditorListItemContent;
	WrapSpacerWidget EditorListItemChildren;
	ButtonWidget EditorListItemCollapse;
	ButtonWidget EditorListItemButton;
		
	void EditorListItem() 
	{ 
		EditorLog.Trace("EditorListItem");
		m_TemplateController.SetListItem(this);
		m_LayoutRoot.SetUserData(this);
	}
	
	void ~EditorListItem() 
	{ 
		EditorLog.Trace("~EditorListItem"); 
	}
	
	void SetNestIndex(int index)
	{
		EditorLog.Trace("EditorListItem::SetNestIndex " + index);
		m_NestIndex = index;
		float x, y;
		EditorListItemButtonFrame.GetSize(x, y);
		EditorListItemButtonFrame.SetSize(290 - 15 * m_NestIndex, y);
	}
	
	int GetNestIndex() {
		return m_NestIndex;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		//EditorLog.Trace("EditorListItemController::OnMouseEnter");
		switch (w.GetName()) {
		
			case "EditorListItemButton": {
				if (m_Selected) {
					EditorListItemContent.SetColor(COLOR_ON_SELECTED);
				} else {
					EditorListItemContent.SetColor(COLOR_ON_HOVER);
				}
				break;
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorListItemController::OnMouseLeave");	
		switch (w.GetName()) {
		
			case "EditorListItemButton": {
				if (m_Selected) {
					EditorListItemContent.SetColor(COLOR_ON_SELECTED);
				} else {
					EditorListItemContent.SetColor(COLOR_ON_DESELECTED);
				}
				break;
			}
		}		
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		return false;
	}

	void ListItemExecute(ButtonCommandArgs args);
	void ListItemVisibleExecute(ButtonCommandArgs args);
	void ListItemCollapseExecute(ButtonCommandArgs args);
	
	private bool m_Selected;
	void Select() 
	{
		//EditorLog.Trace("EditorListItemController::Select");
		m_Selected = true;
		EditorListItemContent.SetColor(COLOR_ON_SELECTED);
	}
	
	void Deselect() 
	{	
		//EditorLog.Trace("EditorListItemController::Deselect");
		m_Selected = false;
		EditorListItemContent.SetColor(COLOR_ON_DESELECTED);
	}
	
	bool IsSelected() {
		return m_Selected;
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/items/EditorListItem.layout";
	}
/*
	override typename GetControllerType() {
		return EditorListItemController;
	}*/
	
	protected EditorListItem GetListItemFromWidget(Widget w)
	{
		EditorListItem target_item;
		w.GetUserData(target_item);
		return target_item;
	}
}
