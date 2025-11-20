class EditorHologram: EditorWorldObject
{
	protected EditorPlaceableItem		m_PlaceableItem;
	protected Widget 					m_EditorMapMarkerWidget;
	protected MapWidget					m_MapWidget;
			
	void EditorHologram(EditorPlaceableItem placeable_item) 
	{
		m_PlaceableItem = placeable_item;
				
		SetWorldObject(CreateObject(placeable_item.GetSpawnType(), vector.Zero));
		GetWorldObject().SetOrientation(vector.Zero); // some objects dont like to orient correctly
	}
	
	void ~EditorHologram()
	{
	}
	
	EditorPlaceableItem GetPlaceableItem() 
	{
		return m_PlaceableItem;
	}
}