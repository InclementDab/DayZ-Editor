class EditorDeletedObject: EditorWorldObject
{
	protected bool m_IsSelected;
	
	protected string m_Type;
	protected vector m_Position;
	protected vector m_Orientation;
	protected vector m_BottomCenter;
		
	private vector m_LineVerticies[8];
	
	protected ref EditorDeletedListItem m_EditorDeletedListItem;
	
	protected ref EditorDeletedObjectWorldMarker m_EditorDeletedObjectWorldMarker;
	
	void EditorDeletedObject(notnull Object object)
	{
		m_WorldObject = object;
		m_Position = m_WorldObject.GetPosition();
		m_Orientation = m_WorldObject.GetOrientation();
		
		vector clip_info[2];
		m_WorldObject.ClippingInfo(clip_info);
		
		m_LineVerticies[0] = clip_info[0];
		m_LineVerticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);		
		m_LineVerticies[4] = Vector(clip_info[1][0], clip_info[1][1], clip_info[0][2]);
		m_LineVerticies[5] = clip_info[1];
		m_LineVerticies[6] = Vector(clip_info[0][0], clip_info[1][1], clip_info[1][2]);
		m_LineVerticies[7] = Vector(clip_info[0][0], clip_info[1][1], clip_info[0][2]);
		
		m_BottomCenter = m_WorldObject.GetGlobalPos(AverageVectors(AverageVectors(m_LineVerticies[0], m_LineVerticies[1]), AverageVectors(m_LineVerticies[2], m_LineVerticies[3])));
		
		// todo: probably use the events system to insert this stuff into the UI
		// use GetListItem()
		m_EditorDeletedListItem = new EditorDeletedListItem(this);
		GetEditor().GetEditorHud().GetTemplateController().RightbarDeletionData.Insert(m_EditorDeletedListItem);
		
		CF.ObjectManager.HideMapObject(m_WorldObject);
	}
	
	void ~EditorDeletedObject()
	{
		// just for the sake of cache clearing
		if (m_IsSelected) {
			OnDeselected();
		}
		
		CF.ObjectManager.UnhideMapObject(m_WorldObject);
		delete m_EditorDeletedListItem;
		delete m_EditorDeletedObjectWorldMarker;
	}
	
	string GetType()
	{
		return m_WorldObject.GetType();
	}
	
	int GetID()
	{
		return m_WorldObject.GetID();
	}
	
	void OnSelected()
	{
		m_IsSelected = true;
		m_EditorDeletedObjectWorldMarker = new EditorDeletedObjectWorldMarker(this);
		if (m_EditorDeletedListItem) {
			m_EditorDeletedListItem.Select();
		}
		
		CF.ObjectManager.UnhideMapObject(m_WorldObject, false);
	}
	
	void OnDeselected()
	{
		m_IsSelected = false;
		delete m_EditorDeletedObjectWorldMarker;
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