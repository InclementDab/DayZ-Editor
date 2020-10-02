

class EditorHologram
{
	protected Widget 					m_EditorMapMarkerWidget;
	protected EditorObjectMapMarker 	m_EditorMapMarker;
	protected MapWidget					m_MapWidget;
	protected EntityAI					m_ProjectionEntity;
	
	EntityAI GetProjectionEntity() 
		return m_ProjectionEntity;
	
	void EditorHologram(string type_name, vector position) 
	{
		m_EditorMapMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorMapMarker.layout", GetEditor().GetEditorHud().EditorMapWidget);
		m_EditorMapMarkerWidget.GetScript(m_EditorMapMarker);
		
		if (!Class.CastTo(m_ProjectionEntity, GetGame().CreateObjectEx(type_name, position, ECE_LOCAL))) {
			EditorLog.Error("EditorHologram: Invalid Object %1", type_name);
			delete this;
		}
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorHologram()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		//GetGame().ObjectDelete(m_ProjectionEntity);
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
		m_ProjectionEntity.PlaceOnSurfaceRotated(mat, position, surface_normal[0] * -1, surface_normal[2] * -1, 0, GetEditor().GetEditorHud().GetEditorHudController().GetToolbarController().MagnetButton);
		m_ProjectionEntity.SetTransform(mat);
		
		
		// -5 for cursor offset
		// -10 to put cursor on center
		//m_EditorMapMarkerWidget.SetPos(map_pos[0] - 15, map_pos[1] - 15);
		

	}
	
	
}