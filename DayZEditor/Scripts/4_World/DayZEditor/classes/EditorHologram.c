

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
		m_WorldObject.SetOrientation(vector.Zero); // some objects dont like to orient correctly
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorHologram()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	void Update(float timeslice)
	{
		if (!m_WorldObject) return;
		vector position = Editor.CurrentMousePosition;
		position[1] = position[1] + ObjectGetSize(m_WorldObject)[1] / 2;
		vector transform[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			position
		};
		
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		float surface_height = GetGame().SurfaceY(position[0], position[2]);
		if (GetEditor().MagnetMode) {
			vector local_ori = m_WorldObject.GetDirection();
			local_ori.Normalize();
			transform[0] = surface_normal * local_ori;
			transform[1] = surface_normal;
			transform[2] = surface_normal * (local_ori * vector.Up);
		}
		
		m_WorldObject.SetTransform(transform);
	}
	
	
}