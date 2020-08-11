class EditorPlacedListItem: EditorListItem
{
	private ref EditorObjectData m_Data;
	EditorObjectData GetData() { return m_Data; }
	
	void EditorPlacedListItem(EditorObjectData data)
	{
		Print("EditorPlacedListItem");
		m_Data = data;
		
		SetText(m_Data.Type);
		SetIcon(Editor.GetIconFromMod(Editor.GetModFromObject(m_Data.Type)));
		
		EditorEvents.OnObjectSelected.Insert(EditorObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	void EditorObjectSelected(Class context, EditorObject target)
	{
		if (target.GetID() == m_Data.GetID())
			SetColor(COLOR_ON_SELECTED);
		
	}
	
	void EditorObjectDeselected(Class context, EditorObject target)
	{		
		if (target.GetID() == m_Data.GetID())
			SetColor(COLOR_ON_DESELECTED); 

	}	
}
