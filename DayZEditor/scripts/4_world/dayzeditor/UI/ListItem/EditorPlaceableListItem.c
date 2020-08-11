class EditorPlaceableListItem: EditorListItem
{
	private ref EditorPlaceableObjectData m_Data;
	EditorPlaceableObjectData GetData() { return m_Data; }

	
	void EditorPlaceableListItem(EditorPlaceableObjectData data)
	{
		Print("EditorPlaceableListItem");
		m_Data = data;
		
		m_ListItemCollapse.Show(false);
		
		SetText(m_Data.Type);
		SetIcon(Editor.GetIconFromMod(Editor.GetModFromObject(m_Data.Type)));
		
		EditorEvents.OnCreateInHand.Insert(OnCreateInHand);
		EditorEvents.OnDestroyInHand.Insert(OnDestroyInHand);
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorPlaceableListItem::OnClick");
		
		if (button == 0) {
			EditorEvents.CreateInHand(this, m_Data.Type);
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
		// We dont want this to be draggable... yet
		return true;
	}
	
	
	
	void OnCreateInHand(Class context, string type)
	{
		if (type == m_Data.Type)
			Select();
	}
	
	void OnDestroyInHand(Class context)
	{
		Deselect();
	}
	
		
	
	
}