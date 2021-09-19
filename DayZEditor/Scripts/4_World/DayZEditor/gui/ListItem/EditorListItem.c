class EditorListItem: ScriptViewTemplate<EditorListItemController>
{
	protected Editor m_Editor = GetEditor();
	protected Widget ListItemContent;
	protected ButtonWidget ListItemShow;
	protected ButtonWidget ListItemFavorites;
	
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
	
		ListItemFavorites.Show(EnableFavorites());
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected()) {
			Deselect();
		}
				
		// only hide when not favorited and disabled
		ListItemFavorites.Show(m_TemplateController.Favorite);
		if (!EnableFavorites()) {
			ListItemFavorites.Show(false);
		}
		
		return true;
	}
	
	bool OnFavoriteToggle(ButtonCommandArgs args)
	{
		return false;
	}
	
	bool EnableFavorites()
	{
		return false;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/items/EditorListItem.layout";
	}
}