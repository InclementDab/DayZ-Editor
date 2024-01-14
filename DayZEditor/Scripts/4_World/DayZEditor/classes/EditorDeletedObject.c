class EditorHiddenObject: Managed
{
	protected bool m_IsSelected;
	
	protected string m_Type;
	protected vector m_Position;
	protected vector m_Orientation;
	protected vector m_BottomCenter;
		
	private vector m_LineVerticies[4];
	
	protected ref EditorDeletedListItem m_EditorDeletedListItem;
	
	protected ref EditorHiddenObjectWorldMarker m_EditorHiddenObjectWorldMarker;
	
	protected EditorHiddenObjectData m_Data;
	protected Object m_WorldObject;
	
	ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();
	
	void EditorHiddenObject(EditorHiddenObjectData data)
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
		
		CF.ObjectManager.HideMapObject(m_WorldObject);
	}
	
	void ~EditorHiddenObject()
	{
		// just for the sake of cache clearing
		if (m_IsSelected) {
			OnDeselected();
		}
		
		CF.ObjectManager.UnhideMapObject(m_WorldObject);
		delete m_EditorDeletedListItem;
		delete m_EditorHiddenObjectWorldMarker;
	}
	
	EditorHiddenObjectData GetData()
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
	
	Object GetWorldObject()
	{
		return m_WorldObject;
	}
	
	void OnSelected()
	{
		m_IsSelected = true;
		m_EditorHiddenObjectWorldMarker = new EditorHiddenObjectWorldMarker(this);
		if (m_EditorDeletedListItem) {
			m_EditorDeletedListItem.Select();
		}
		
		CF.ObjectManager.UnhideMapObject(m_WorldObject, false);
	}
	
	void OnDeselected()
	{
		m_IsSelected = false;
		delete m_EditorHiddenObjectWorldMarker;
		if (m_EditorDeletedListItem) {
			m_EditorDeletedListItem.Deselect();
		}
		
		CF.ObjectManager.HideMapObject(m_WorldObject, false);
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