class EditorPlaceableTreeItem: EditorTreeItem
{
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case Panel: {				
				Widget scroller = m_LayoutRoot;
				if (RecursiveGetParent(scroller, ScrollWidget)) {
					float w_f, h_f, x_p, y_p;
					scroller.GetScreenSize(w_f, h_f);
					scroller.GetScreenPos(x_p, y_p);
					
					EditorPlaceableObjectData data = EditorPlaceableObject.Cast(m_Selectable).GetPlaceableObjectData();
					
					if (!IsBlacklistedItem(data.GetName())) {
						//EditorHud.CurrentTooltip = new EditorObjectTooltip(data.CreateObject(vector.Zero, vector.Zero, 1.0, ECE_LOCAL), x_p + w_f + 25, y);
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
				delete GetEditor().GetHud().CurrentTooltip;
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	static bool IsBlacklistedItem(string item_type)
	{
		array<string> blacklist = { "DZ_LightAI", "Man", "Car" };
		foreach (string blacklist_check: blacklist) {
			if (GetGame().IsKindOf(item_type, blacklist_check)) {
				return true;
			}
		}
		
		return false;
	}
}