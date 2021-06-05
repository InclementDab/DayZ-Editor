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
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected()) {
			Deselect();
		}
		
		return true;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/items/EditorListItem.layout";
	}
}