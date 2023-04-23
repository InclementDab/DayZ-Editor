class EditorHologram: EditorWorldObject
{
	protected EditorPlaceableItem		m_PlaceableItem;
	protected Widget 					m_EditorMapMarkerWidget;
	protected EditorObjectMapMarker 	m_EditorMapMarker;
	protected MapWidget					m_MapWidget;
			
	void EditorHologram(EditorPlaceableItem placeable_item) 
	{
		m_PlaceableItem = placeable_item;
				
		m_WorldObject = CreateObject(placeable_item.GetSpawnType(), Editor.CurrentMousePosition);
		m_WorldObject.SetOrientation(vector.Zero); // some objects dont like to orient correctly
	}
	
	void ~EditorHologram()
	{
	}
	
	EditorPlaceableItem GetPlaceableItem() 
	{
		return m_PlaceableItem;
	}
}