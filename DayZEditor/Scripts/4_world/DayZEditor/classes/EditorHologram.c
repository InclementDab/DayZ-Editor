

class EditorHologram: EditorWorldObject
{
	protected EditorPlaceableItem		m_PlaceableItem;
	protected Widget 					m_EditorMapMarkerWidget;
	protected EditorObjectMapMarker 	m_EditorMapMarker;
	protected MapWidget					m_MapWidget;
		
	EditorPlaceableItem GetPlaceableItem() {
		return m_PlaceableItem;
	}
	
	void EditorHologram(EditorPlaceableItem placeable_item) 
	{
		m_PlaceableItem = placeable_item;
		//m_EditorMapMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorMapMarker.layout", GetEditor().GetEditorHud().EditorMapWidget);
		//m_EditorMapMarkerWidget.GetScript(m_EditorMapMarker);
		
		m_WorldObject = CreateObject(placeable_item.Type);

		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorHologram()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	void Update(float timeslice)
	{
		vector position = Editor.CurrentMousePosition;
		vector mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			position
		};
		
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		float surface_height = GetGame().SurfaceY(position[0], position[2]);
		m_WorldObject.PlaceOnSurfaceRotated(mat, position, surface_normal[0] * -1, surface_normal[2] * -1, 0, GetEditor().MagnetMode);
		m_WorldObject.SetTransform(mat);
		
		
		// -5 for cursor offset
		// -10 to put cursor on center
		//m_EditorMapMarkerWidget.SetPos(map_pos[0] - 15, map_pos[1] - 15);
		

	}
	
	
}