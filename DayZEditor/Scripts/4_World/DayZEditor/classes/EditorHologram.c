class EditorHologram: EditorWorldObject
{
	protected EditorPlaceableItem		m_PlaceableItem;
	protected Widget 					m_EditorMapMarkerWidget;
	protected EditorObjectMapMarker 	m_EditorMapMarker;
	protected MapWidget					m_MapWidget;
			
	void EditorHologram(EditorPlaceableItem placeable_item) 
	{
		m_PlaceableItem = placeable_item;
		
		m_WorldObject = placeable_item.CreateObject(Editor.CurrentMousePosition, vector.Zero);
	}
		
	EditorPlaceableItem GetPlaceableItem() 
	{
		return m_PlaceableItem;
	}
}