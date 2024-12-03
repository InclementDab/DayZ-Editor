class EditorDeletedObject: EditorWorldObject
{
	protected bool m_IsSelected;
	
	protected string m_Type;
	protected vector m_Position;
	protected vector m_Orientation;
	protected vector m_BottomCenter;
		
	private vector m_LineVerticies[4];
	
	protected ref EditorDeletedListItem m_EditorDeletedListItem;
	
	protected ref EditorDeletedObjectWorldMarker m_EditorDeletedObjectWorldMarker;
	
	protected EditorDeletedObjectData m_Data;
	
	void EditorDeletedObject(EditorDeletedObjectData data)
	{
		m_Data = data;
				
		m_WorldObject = m_Data.WorldObject;
		if (!m_WorldObject) {
			m_WorldObject = m_Data.FindObject();
			if (!m_WorldObject) {
				EditorLog.Error("Failed to find object with name %1 at position %2", m_Data.Type, m_Data.Position.ToString());
				return;
			}
		}
		
		m_Position = m_WorldObject.GetPosition();
		m_Orientation = m_WorldObject.GetOrientation();
		
		vector clip_info[2];
		m_WorldObject.ClippingInfo(clip_info);
		
		m_LineVerticies[0] = clip_info[0];
		m_LineVerticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);		
		
		m_BottomCenter = m_WorldObject.GetGlobalPos(AverageVectors(AverageVectors(m_LineVerticies[0], m_LineVerticies[1]), AverageVectors(m_LineVerticies[2], m_LineVerticies[3])));
		
		// todo: probably use the events system to insert this stuff into the UI
		m_EditorDeletedListItem = new EditorDeletedListItem(this);		
		GetEditor().GetEditorHud().GetTemplateController().RightbarDeletionData.Insert(m_EditorDeletedListItem);
		
		GetDayZGame().GetSuppressedObjectManager().Suppress(m_WorldObject);
		
		m_EditorDeletedObjectWorldMarker = new EditorDeletedObjectWorldMarker(this);
		m_EditorDeletedObjectWorldMarker.Show(false);
	}
	
	void ~EditorDeletedObject()
	{
		// just for the sake of cache clearing
		if (m_IsSelected) {
			OnDeselected();
		}
		
		if (GetDayZGame() && GetDayZGame().GetSuppressedObjectManager()) {
			GetDayZGame().GetSuppressedObjectManager().Unsupress(m_WorldObject);
		}
		
		delete m_EditorDeletedListItem;
	}
	
	EditorDeletedObjectData GetData()
	{
		return m_Data;
	}
	
	string GetType()
	{
		return m_Data.Type;
	}
	
	int GetID()
	{
		return m_Data.ID;
	}
	
	void OnSelected()
	{
		m_IsSelected = true;
		
		if (m_EditorDeletedListItem) {
			m_EditorDeletedListItem.Select();
		}
		
		// Temporarily unsuppress
		GetDayZGame().GetSuppressedObjectManager().Unsupress(m_WorldObject);
	}
	
	void OnDeselected()
	{
		m_IsSelected = false;
		
		if (m_EditorDeletedListItem) {
			m_EditorDeletedListItem.Deselect();
		}
		
		GetDayZGame().GetSuppressedObjectManager().Suppress(m_WorldObject);
	}
	
	bool IsSelected()
	{
		return m_IsSelected;
	}
	
	vector GetOriginalPosition()
	{
		return m_Position;
	}
	
	vector GetOriginalOrientation()
	{
		return m_Orientation;
	}
	
	vector GetBottomPosition()
	{
		return m_BottomCenter;
	}
	
	EditorDeletedListItem GetListItem()
	{
		return m_EditorDeletedListItem;
	}
}