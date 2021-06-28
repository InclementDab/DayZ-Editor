class EditorListItem: ScriptViewTemplate<EditorListItemController>
{
	protected Editor m_Editor = GetEditor();
	protected Widget ListItemContent;
	protected ButtonWidget ListItemShow;
	
	void Select() 
	{
		ListItemContent.SetColor(m_Editor.Settings.SelectionColor);
	}
	
	void Highlight()
	{
		ListItemContent.SetColor(m_Editor.Settings.HighlightColor);
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