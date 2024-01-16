class EditorListItem: ScriptViewTemplate<EditorListItemController>
{
	protected Editor m_Editor = GetEditor();
	protected WrapSpacerWidget ListItem;
	protected ButtonWidget ListItemShow;
	
	// Can be null!
	protected ButtonWidget ListItemFavorites;
	
	void Select() 
	{
		ListItem.SetColor(m_Editor.GeneralSettings.SelectionColor);
	}
	
	void Highlight()
	{
		ListItem.SetColor(m_Editor.GeneralSettings.HighlightColor);
	}
	
	void Deselect() 
	{	
		ListItem.SetColor(COLOR_EMPTY);
	}
	
	// Abstract
	bool IsSelected();
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!IsSelected()) {
			Highlight();
		}
	
		if (ListItemFavorites) {
			ListItemFavorites.Show(EnableFavorites());
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected()) {
			Deselect();
		}
				
		// only hide when not favorited and disabled
		if (ListItemFavorites) {
			ListItemFavorites.Show(m_TemplateController.Favorite);
			if (!EnableFavorites()) {
				ListItemFavorites.Show(false);
			}
		}
		
		return true;
	}
	
	void SetFavorite(bool favorite)
	{
		if (ListItemFavorites) {
			ListItemFavorites.Show(favorite);
		}
		
		GetTemplateController().Favorite = favorite;
		GetTemplateController().NotifyPropertyChanged("Favorite");
	}
	
	bool OnFavoriteToggle(CheckBoxCommandArgs args)
	{
		return false;
	}
	
	protected bool EnableFavorites()
	{
		return false;
	}
	
	// abstract
	bool FilterType(string filter);
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/items/EditorListItem.layout";
	}
}