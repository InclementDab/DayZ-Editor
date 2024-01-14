class EditorItemTreeItem: EditorTreeItem
{
	protected EditorPlaceableItem m_PlaceableItem;
	protected Object m_Hologram;
	
	void EditorItemTreeItem(EditorPlaceableItem placeable_item)
	{
		m_PlaceableItem = placeable_item;
		
		m_TemplateController.Text = m_PlaceableItem.GetName();
		m_TemplateController.NotifyPropertyChanged("Text");
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		switch (w) {
			case Panel: {
				WidgetAnimator.AnimateColor(Panel, ARGB(255, 75, 119, 190), 50);
				m_Hologram = m_PlaceableItem.CreateObject(Editor.CurrentMousePosition, vector.Zero, 1.0);
				GetEditor().AddInHand(m_Hologram);
				return true;
			}
		}
		
		return super.OnFocus(w, x, y);
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		switch (w) {
			case Panel: {
				WidgetAnimator.AnimateColor(Panel, ARGB(0, 0, 0, 0), 50);
				break;
			}
		}
		
		return super.OnFocusLost(w, x, y);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case Panel: {				
				Widget scroller = m_LayoutRoot;
				if (RecursiveGetParent(scroller, ScrollWidget)) {
					float w_f, h_f, x_p, y_p;
					scroller.GetScreenSize(w_f, h_f);
					scroller.GetScreenPos(x_p, y_p);
					EditorPlaceableTooltip tooltip = new EditorPlaceableTooltip(m_PlaceableItem, x_p + w_f + 25, y);
					GetEditor().GetEditorHud().SetCurrentTooltip(tooltip);
				}
				
				break;
			}
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case Panel: {
				GetEditor().GetEditorHud().SetCurrentTooltip(null);
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
}