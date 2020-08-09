

class EditorHologram: Hologram
{
	protected Widget 				m_EditorMapMarkerWidget;
	protected EditorMapMarker 		m_EditorMapMarker;
	
	private MapWidget				m_MapWidget;
	
	void EditorHologram(PlayerBase player, vector pos, ItemBase item) 
	{
		m_EditorMapMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorMapMarker.layout");
		m_EditorMapMarkerWidget.GetScript(m_EditorMapMarker);
		

		GetEditor().GetUIManager().GetEditorUI().InsertMapObject(m_EditorMapMarkerWidget);
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorHologram()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		delete m_EditorMapMarkerWidget; delete m_EditorMapMarker;
	}
	
	void Update(float timeslice)
	{		
		int x, y;
		GetCursorPos(x, y);
		if (m_Projection == NULL) return;
		
		// Handle Building
		if (GetEditor().GetUIManager().GetEditorUI().IsMapOpen()) {
			vector pos = m_MapWidget.ScreenToMap(Vector(x, y, 0));
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);	
				
		} else {
			set<Object> obj;
			pos = MousePosToRay(obj, m_Projection);
		}
		

		
		vector mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			pos
		};
		
		vector surface_normal = GetGame().SurfaceGetNormal(pos[0], pos[2]);
		float surface_height = GetGame().SurfaceY(pos[0], pos[2]);
		m_Projection.PlaceOnSurfaceRotated(mat, pos, surface_normal[0] * -1, surface_normal[2] * -1, 0, EditorSettings.MAGNET_PLACEMENT);
		m_Projection.SetTransform(mat);
		
				
		// Handle Map Marker
		vector map_pos = m_MapWidget.MapToScreen(pos);
		// -5 for cursor offset
		// -10 to put cursor on center
		m_EditorMapMarkerWidget.SetPos(map_pos[0] - 15, map_pos[1] - 15);
		

	}
	
	
}