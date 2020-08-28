typedef ref array<ref EditorPlaceableListItem> EditorPlaceableListItemSet;





class EditorPlaceableListItem: EditorListItem
{
	private ref EditorPlaceableObjectData m_Data;
	EditorPlaceableObjectData GetData() { 
		return m_Data; 
	}

	
	void EditorPlaceableListItem(ref EditorPlaceableObjectData data) 
	{ 
		EditorLog.Trace("EditorPlaceableListItem"); 
		m_Data = data;
		
		m_EditorListItemController.SetLabel(m_Data.Type);
		m_EditorListItemController.SetIcon(GetIconFromMod(GetModFromObject(m_Data.Type)));
		
		EditorEvents.OnStartPlacing.Insert(StartPlacing);
		EditorEvents.OnStopPlacing.Insert(StopPlacing);
	}
	

	/*
		

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
	*/
	
	
	void StartPlacing(Class context, EditorPlaceableObjectData type)
	{
		if (type.Type == m_Data.Type)
			m_EditorListItemController.Select();
		else m_EditorListItemController.Deselect();
	}
	
	void StopPlacing(Class context)
	{
		m_EditorListItemController.Deselect();
	}
	
		
	
	
}