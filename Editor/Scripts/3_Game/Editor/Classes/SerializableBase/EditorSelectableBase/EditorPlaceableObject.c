class EditorPlaceableObject: EditorNode
{
	protected EditorPlaceableObjectData m_PlaceableObject;
	protected Object m_Object;
		
    void EditorPlaceableObject()
    {
        m_PlaceableObject = placeable_object;
		
		m_TreeItem = new EditorPlaceableTreeItem(m_PlaceableObject.GetName(), this);
		m_TreeItem.SetIcon(m_PlaceableObject.GetIcon());
    }
	
	override EditorNodeView GetTreeItem()
	{
		return m_TreeItem;
	}

	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {
			vector transform[4];
			Math3D.MatrixIdentity4(transform);
			
			
			Ray ray = GetDayZGame().GetEditor().GetCamera().PerformCursorRaycast();
			transform[3] = ray.Position;
			
			GetDayZGame().GetEditor().Placing[m_PlaceableObject.CreateObject(transform)] = new EditorHandData();
		}
	}
	
	EditorPlaceableObjectData GetPlaceableObjectData()
	{
		return m_PlaceableObject;
	}
}