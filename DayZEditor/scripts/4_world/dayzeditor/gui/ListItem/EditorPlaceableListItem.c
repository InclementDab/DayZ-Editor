typedef ref array<ref EditorPlaceableListItem> EditorPlaceableListItemSet;

class EditorPlaceableListItem: EditorListItem
{
	private ref EditorPlaceableObjectData m_Data;
	EditorPlaceableObjectData GetData() { return m_Data; }

	
	void EditorPlaceableListItem(ref EditorPlaceableObjectData data) 
	{ 
		EditorLog.Trace("EditorPlaceableListItem"); 
		m_Data = data;

		m_ListItemCollapse.Show(false);

		SetText(m_Data.Type);
		SetIcon(GetIconFromMod(GetModFromObject(m_Data.Type)));
		
		EditorEvents.OnStartPlacing.Insert(StartPlacing);
		EditorEvents.OnStopPlacing.Insert(StopPlacing);
	}
	
		
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnClick");
		
		if (button == 0) {
			GetEditor().CreateInHand(m_Data);
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
		GetEditor().CreateInHand(m_Data);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		GetEditor().PlaceObject();
		return true;
	}
	
	
	
	void StartPlacing(Class context, EditorPlaceableObjectData type)
	{
		if (type == m_Data.Type)
			Select();
	}
	
	void StopPlacing(Class context)
	{
		Deselect();
	}
	
		
	
	
}