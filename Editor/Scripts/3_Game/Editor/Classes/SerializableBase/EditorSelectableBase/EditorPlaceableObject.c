class EditorPlaceableObject: EditorSelectableBase
{
	protected EditorPlaceableObjectData m_PlaceableObject;
	protected Object m_Object;
		
    void EditorPlaceableObject(notnull EditorPlaceableObjectData placeable_object)
    {
        m_PlaceableObject = placeable_object;
		
		m_TreeItem = new EditorPlaceableTreeItem(m_PlaceableObject.GetName(), this);
    }
	
	override EditorTreeItem GetTreeItem()
	{
		return m_TreeItem;
	}

	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {
			m_Object = m_PlaceableObject.CreateObject(vector.Zero, vector.Zero, 1.0, ECE_LOCAL);
			GetEditor().AddInHand(m_Object, new EditorHandData());
		} else {
			GetEditor().RemoveFromHand(m_Object);
			GetGame().ObjectDelete(m_Object);
		}
	}
	
	EditorPlaceableObjectData GetPlaceableObjectData()
	{
		return m_PlaceableObject;
	}
}