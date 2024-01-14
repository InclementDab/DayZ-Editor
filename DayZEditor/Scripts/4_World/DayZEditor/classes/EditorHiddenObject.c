class EditorHiddenObject: Managed
{
	protected bool m_IsSelected;
	
	protected string m_Type;
	protected vector m_Position, m_Orientation, m_BottomCenter;
		
	private vector m_LineVerticies[4];
	
	protected ref EditorDeletedListItem m_EditorDeletedListItem;
	
	protected ref EditorHiddenObjectWorldMarker m_EditorHiddenObjectWorldMarker;
	
	protected Object m_Object;
	
	static ref array<EditorHiddenObject> SelectedObjects = {};
	
	ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();
	
	void EditorHiddenObject(notnull Object object)
	{
		m_Object = object;
					
		vector clip_info[2];
		m_Object.ClippingInfo(clip_info);
		
		m_LineVerticies[0] = clip_info[0];
		m_LineVerticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);		
		
		m_BottomCenter = m_Object.GetGlobalPos(AverageVectors(AverageVectors(m_LineVerticies[0], m_LineVerticies[1]), AverageVectors(m_LineVerticies[2], m_LineVerticies[3])));
		
		// todo: probably use the events system to insert this stuff into the UI
		m_EditorDeletedListItem = new EditorDeletedListItem(this);		
		GetEditor().GetEditorHud().GetTemplateController().RightbarDeletionData.Insert(m_EditorDeletedListItem);
		
		CF.ObjectManager.HideMapObject(m_Object);
	}
	
	void ~EditorHiddenObject()
	{		
		CF.ObjectManager.UnhideMapObject(m_Object);
	}
	
	static EditorHiddenObject CreateFromSerializer(notnull EditorHiddenObjectData object_data)
	{
		EditorHiddenObject hidden_object = new EditorHiddenObject(object_data.FindObject());
		
		return hidden_object;
	}
	
	EditorHiddenObjectData CreateSerializedData()
	{
		EditorHiddenObjectData data = new EditorHiddenObjectData();
		data.Type = m_Object.GetType();
		data.Position = m_Object.GetPosition();
		data.Orientation = m_Object.GetOrientation();
		//data.Flags = m_Flags;
				
		return data;
	}
	
	void SetSelected(bool selected)
	{
		m_IsSelected = selected;
		if (selected) {
			CF.ObjectManager.UnhideMapObject(m_Object, false);
			SelectedObjects.Insert(this);
			OnObjectSelected.Invoke(this);
		} else {
			CF.ObjectManager.HideMapObject(m_Object, false);
			SelectedObjects.RemoveItem(this);
			OnObjectDeselected.Invoke(this);
		}
	}
	
	bool IsSelected() 
	{
		return m_IsSelected;
	}
	
	EditorHiddenObjectData GetData()
	{
		return m_Data;
	}
		
	Object GetObject()
	{
		return m_Object;
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