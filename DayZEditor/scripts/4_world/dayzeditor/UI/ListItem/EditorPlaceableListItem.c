class EditorPlaceableListItem: EditorListItem
{
	private ref EditorPlaceableObjectData m_Data;
	EditorPlaceableObjectData GetData() { return m_Data; }

	
	void EditorPlaceableListItem(EditorPlaceableObjectData data)
	{
		Print("EditorPlaceableListItem");
		m_Data = data;
		
		SetText(m_Data.Type);
		SetIcon(Editor.GetIconFromMod(Editor.GetModFromObject(m_Data.Type)));
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorPlaceableListItem::OnClick");
		
		if (button == 0) {
			GetEditor().CreateObjectInHand(m_Data.Type);
			SetFocus(w);
			return true;
		} else if (button == 1) {
			
			if (GetGame().GetInput().LocalValue("UAWalkRunTemp")) {				
				// all very temporary please abstract elsewhere
				if (GetEditor().IsLootEditActive())
					GetEditor().PlaceholderRemoveLootMode();
				else 
					GetEditor().PlaceholderForEditLootSpawns(m_Data.Type);
				
				return true;				
			}
		}

		
		return false;
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		// We dont want this to be draggable
		return true;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorPlaceableListItem::OnFocus");
		SetColor(COLOR_ON_SELECTED);
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorPlaceableListItem::OnFocusLost");
		SetColor(COLOR_ON_DESELECTED);
		return true;
	}
	
}