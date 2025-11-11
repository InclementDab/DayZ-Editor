class EditorDeletedObject: EditorWorldObject
{
	string Uuid;
	
	protected bool m_IsSelected;
	
	protected string m_Type;
	protected vector m_Position;
	protected vector m_Orientation;
	protected vector m_BottomCenter;
	
	protected vector m_Transform[4];
			
	protected ref EditorDeletedListItem m_EditorDeletedListItem;
	protected ref EditorDeletedObjectWorldMarker m_EditorDeletedObjectWorldMarker;
	
	protected EditorDeletedObjectData m_Data;
	
	void EditorDeletedObject(EditorDeletedObjectData data)
	{
		m_Data = data;
		
		// Bugfix for not having the world object info loaded yet
		m_Data.WorldObject = m_Data.FindObject();
		
		SetWorldObject(m_Data.WorldObject);
		if (!GetWorldObject()) {
			SetWorldObject(m_Data.FindObject());
			if (!GetWorldObject()) {
				EditorLog.Error("Failed to find object with name %1:%3 at position %2", m_Data.Type, m_Data.Position.ToString(), m_Data.ModelName);
				return;
			}
		}
		
		m_Position = GetWorldObject().GetPosition();
		m_Orientation = GetWorldObject().GetOrientation();
		
		// Store original transform
		vector transform[4];
		m_WorldObject.GetTransform(transform);
		copyarray(m_Transform, transform);
		
		/*
		vector clip_info[2];
		GetWorldObject().ClippingInfo(clip_info);
		
		m_LineVerticies[0] = clip_info[0];
		m_LineVerticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);*/
		
		m_BottomCenter = GetWorldObject().GetGlobalPos(AverageVectors(AverageVectors(m_LineVerticies[0], m_LineVerticies[1]), AverageVectors(m_LineVerticies[2], m_LineVerticies[3])));
		
		// todo: probably use the events system to insert this stuff into the UI
		m_EditorDeletedListItem = new EditorDeletedListItem(this);		
		GetEditor().GetEditorHud().GetTemplateController().RightbarDeletionData.Insert(m_EditorDeletedListItem);
		
		g_Game.GetSuppressedObjectManager().Suppress(GetWorldObject());
		
		m_EditorDeletedObjectWorldMarker = new EditorDeletedObjectWorldMarker(this);
		m_EditorDeletedObjectWorldMarker.Show(false);
	}
	
	void ~EditorDeletedObject()
	{
		// just for the sake of cache clearing
		if (m_IsSelected) {
			OnDeselected();
		}
		
		if (g_Game && g_Game.GetSuppressedObjectManager()) {
			g_Game.GetSuppressedObjectManager().Unsupress(GetWorldObject());
		}
		
		delete m_EditorDeletedListItem;
	}
	
	override void GetTransform(out vector mat[4]) 
	{ 
		copyarray(mat, m_Transform);
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
		
		if (m_EditorDeletedObjectWorldMarker) {
			m_EditorDeletedObjectWorldMarker.Show(true);
		}
		
		// Temporarily unsuppress
		g_Game.GetSuppressedObjectManager().Unsupress(GetWorldObject());
	}
	
	void OnDeselected()
	{
		m_IsSelected = false;
		
		if (m_EditorDeletedListItem) {
			m_EditorDeletedListItem.Deselect();
		}
		
		if (m_EditorDeletedObjectWorldMarker) {
			m_EditorDeletedObjectWorldMarker.Show(false);
		}
		
		g_Game.GetSuppressedObjectManager().Suppress(GetWorldObject());
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