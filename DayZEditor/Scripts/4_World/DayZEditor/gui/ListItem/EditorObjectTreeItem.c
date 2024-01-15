class EditorObjectTreeItem: EditorTreeItem
{
	protected EditorObject m_EditorObject;
	
	void EditorObjectTreeItem(EditorObject editor_object)
	{
		m_EditorObject = editor_object;
		
		m_TemplateController.Text = m_EditorObject.GetDisplayName();
		m_TemplateController.NotifyPropertyChanged("Text");
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		switch (w) {
			case Panel: {
				WidgetAnimator.AnimateColor(Panel, ARGB(255, 75, 119, 190), 50);
				//GetEditor().AddInHand(m_EditorObject);
				m_EditorObject.SetSelected(true);
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
				m_EditorObject.SetSelected(false);
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
					
					if (m_EditorObject.GetWorldObject() && !IsBlacklistedItem(m_EditorObject.GetWorldObject().GetType())) {
						//GetEditor().GetEditorHud().SetCurrentTooltip(new EditorObjectTooltip(m_EditorObject.GetWorldObject(), x_p + w_f + 25, y));
					}
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
				//GetEditor().GetEditorHud().SetCurrentTooltip(null);
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
}