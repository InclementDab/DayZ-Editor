

class EditorHologram
{
	protected Widget 				m_EditorMapMarkerWidget;
	protected EditorObjectMapMarker 		m_EditorMapMarker;
	
	private MapWidget				m_MapWidget;
	
	private EntityAI				m_ProjectionEntity;
	EntityAI GetProjectionEntity() { return m_ProjectionEntity; }
	
	void EditorHologram(string type_name, vector position) 
	{
		m_MapWidget = GetEditor().GetEditorHud().GetMapWidget();
		m_EditorMapMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorMapMarker.layout", m_MapWidget);
		m_EditorMapMarkerWidget.GetScript(m_EditorMapMarker);
		
		m_ProjectionEntity = GetGame().CreateObjectEx(type_name, position, ECE_LOCAL);
		
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorHologram()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		GetGame().ObjectDelete(m_ProjectionEntity);
	}
	
	void Update(float timeslice)
	{		
		int x, y;
		GetMousePos(x, y);
		
		// Handle Building
		if (GetEditor().GetEditorHud().IsMapVisible()) {
			vector pos = m_MapWidget.ScreenToMap(Vector(x, y, 0));
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);	
				
		} else {
			set<Object> obj;
			pos = MousePosToRay(obj, m_ProjectionEntity);
		}
				
		vector mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			pos
		};
		
		vector surface_normal = GetGame().SurfaceGetNormal(pos[0], pos[2]);
		float surface_height = GetGame().SurfaceY(pos[0], pos[2]);
		m_ProjectionEntity.PlaceOnSurfaceRotated(mat, pos, surface_normal[0] * -1, surface_normal[2] * -1, 0, EditorSettings.MAGNET_PLACEMENT);
		m_ProjectionEntity.SetTransform(mat);
		
				
		// Handle Map Marker
		vector map_pos = m_MapWidget.MapToScreen(pos);
		// -5 for cursor offset
		// -10 to put cursor on center
		m_EditorMapMarkerWidget.SetPos(map_pos[0] - 15, map_pos[1] - 15);
		

	}
	
	
}