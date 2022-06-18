class EditorHologram: EditorWorldObject
{
	protected EditorPlaceableItem		m_PlaceableItem;
	protected Widget 					m_EditorMapMarkerWidget;
	protected EditorObjectMapMarker 	m_EditorMapMarker;
	protected MapWidget					m_MapWidget;
			
	void EditorHologram(EditorPlaceableItem placeable_item) 
	{
		m_PlaceableItem = placeable_item;
		//m_EditorMapMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorMapMarker.layout", GetEditor().GetEditorHud().EditorMapWidget);
		//m_EditorMapMarkerWidget.GetScript(m_EditorMapMarker);
		
		m_WorldObject = CreateObject(placeable_item.Type, Editor.CurrentMousePosition);
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