class EditorDeletedObject: EditorWorldObject
{
	protected bool m_IsSelected;
	protected vector m_OriginalPosition;
	protected vector m_OriginalOrientation;
	protected vector m_BottomCenter;
	
	private vector m_LineVerticies[8];
	
	protected ref EditorDeletedListItem m_EditorDeletedListItem;
	
	protected ref EditorDeletedObjectWorldMarker m_EditorDeletedObjectWorldMarker;
	
	void EditorDeletedObject(notnull Object object)
	{
		m_WorldObject = object;
		m_OriginalPosition = m_WorldObject.GetPosition();
		m_OriginalOrientation = m_WorldObject.GetOrientation();
		
		
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
		
		m_BottomCenter = AverageVectors(AverageVectors(m_LineVerticies[0], m_LineVerticies[1]), AverageVectors(m_LineVerticies[2], m_LineVerticies[3]));
		
		// todo: probably use the events system to insert this stuff into the UI
		// use GetListItem()
		m_EditorDeletedListItem = new EditorDeletedListItem(this);
		GetEditor().GetEditorHud().GetTemplateController().RightbarDeletionData.Insert(m_EditorDeletedListItem);
		
		CF.ObjectManager.HideMapObject(m_WorldObject);
	}
	
	void ~EditorDeletedObject()
	{
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
	}
	
	void OnDeselected()
	{
		m_IsSelected = false;
		delete m_EditorDeletedObjectWorldMarker;
		if (m_EditorDeletedListItem) {
			m_EditorDeletedListItem.Deselect();
		}
	}
	
	bool IsSelected()
	{
		return m_IsSelected;
	}
	
	vector GetOriginalPosition()
	{
		return m_OriginalPosition;
	}
	
	vector GetOriginalOrientation()
	{
		return m_OriginalOrientation;
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