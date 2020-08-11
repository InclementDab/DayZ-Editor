class EditorPlaceableListItem: EditorListItem
{
	private ref EditorPlaceableObjectData m_Data;
	EditorPlaceableObjectData GetData() { return m_Data; }

	
	void EditorPlaceableListItem() { Print("EditorPlaceableListItem"); }
	
	private void pCreate(EditorPlaceableObjectData data)
	{
		Print("EditorPlaceableListItem::pCreate");
		m_ListItemCollapse.Show(false);

		SetText(m_Data.Type);
		SetIcon(Editor.GetIconFromMod(Editor.GetModFromObject(m_Data.Type)));
		
		EditorEvents.OnCreateInHand.Insert(OnCreateInHand);
		EditorEvents.OnDestroyInHand.Insert(OnDestroyInHand);
	}
	
	static EditorPlaceableListItem Create(EditorPlaceableObjectData data)
	{
		Print("EditorPlaceableListItem::Create");
		EditorPlaceableListItem p_item;
		EditorListItem item;
		Widget w = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/items/EditorListItem.layout", null);
		w.GetScript(item);
		
		Print(p_item);
		return item;
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
		EditorEvents.CreateInHand(this, m_Data.Type);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		GetEditor().PlaceObject();
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