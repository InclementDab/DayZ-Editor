
class EditorListItem: ScriptViewTemplate<EditorListItemController>
{
	protected Widget ListItemContent;
	protected ButtonWidget ListItemShow;
	
	void Select() 
	{
		ListItemContent.SetColor(LIST_ITEM_COLOR_ON_SELECTED);
	}
	
	void Highlight()
	{
		ListItemContent.SetColor(LIST_ITEM_COLOR_ON_HOVER);
	}
	
	void Deselect() 
	{	
		ListItemContent.SetColor(COLOR_EMPTY);
	}
	
	// Abstract
	bool IsSelected();
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!IsSelected()) {
			Highlight();
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected()) {
			Deselect();
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/items/EditorListItem.layout";
	}
}


/*
class EditorListItem: ScriptViewTemplate<EditorListItemController>
{
	protected int m_NestIndex;
	
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

	
	protected EditorListItem GetListItemFromWidget(Widget w)
	{
		EditorListItem target_item;
		w.GetUserData(target_item);
		return target_item;
	}
}

	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		if (reciever == m_ListItem.EditorListItemButton && m_ListItem.Type() == EditorCollapsibleListItem) {
			ScriptedViewBase scripted_view;
			w.GetScript(scripted_view);
			ScriptedViewBase.FindScriptedRoot(scripted_view);
			
			reciever = m_ListItem.EditorListItemChildren;
			ViewBinding view_binding;
			reciever.GetScript(view_binding);
			
			if (view_binding) {
				
				// Checks for the child-in-parent deal
				if (!RecursiveCheckChildren(m_ListItem, ChildListItems)) {
					view_binding.OnDropReceived(w, x, y, reciever);
				}
				return true;
			}
		}
				
		return false;
	}
	
	private bool RecursiveCheckChildren(EditorListItem target, out ObservableCollection<ref EditorListItem> check_list)
	{	
		Print(check_list.Count());
		for (int i = 0; i < check_list.Count(); i++) {
			Print(check_list[i]);
			/*if (check_list[i] == target) {
				return true;
			}
			
			if (check_list[i].Type() == EditorCollapsibleListItem) {
				if (RecursiveCheckChildren(target, check_list[i].GetListItemController().ChildListItems)) {
					return true;
				}
			}
		}
		
		return false;
	}

	private bool AllChildrenSelected()
	{
		for (int i = 0; i < ChildListItems.Count(); i++) {
			EditorListItem list_item = EditorListItem.Cast(ChildListItems[i]);
			if (!list_item.IsSelected()) {
				return false;
			}
		}
		
		return true;
	}
*/