class EditorPlaceableTreeItem: EditorTreeItem
{	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case Button: {				
				Widget scroller = m_LayoutRoot;
				if (RecursiveGetParent(scroller, ScrollWidget)) {
					float w_f, h_f, x_p, y_p;
					scroller.GetScreenSize(w_f, h_f);
					scroller.GetScreenPos(x_p, y_p);
					
					//if (!IsBlacklistedItem(m_PlaceableItem.GetName())) {
						//EditorObjectTooltip tooltip = new EditorObjectTooltip(m_PlaceableItem.CreateObject(vector.Zero, vector.Zero, 1.0, ECE_LOCAL), x_p + w_f + 25, y);
						//GetEditor().GetEditorHud().SetCurrentTooltip(tooltip);
					//}
				}
				
				break;
			}
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case Button: {
				//GetEditor().GetEditorHud().SetCurrentTooltip(null);
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
}