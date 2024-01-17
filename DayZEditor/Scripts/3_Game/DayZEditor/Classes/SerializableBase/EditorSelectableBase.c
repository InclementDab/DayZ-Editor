class EditorSelectableBase: SerializableBase
{
	ref ScriptInvoker OnSelectionChanged = new ScriptInvoker();
	
	protected bool m_IsSelected;
		
	protected ref EditorTreeItem m_TreeItem;
	
	EditorTreeItem GetTreeItem()
	{
		return m_TreeItem;
	}
	
	void SetSelected(bool selected)
	{
		m_IsSelected = selected;
		
		if (m_IsSelected) {
			SelectedObjects.Insert(this);
		} else {
			SelectedObjects.RemoveItem(this);
		}
				
		OnSelectionChanged.Invoke(this, m_IsSelected);
	}
	
	bool IsSelected() 
	{
		return m_IsSelected;
	}
	
	string GetModel()
	{
		return string.Empty;
	}
	
	string GetName()
	{
		return string.Empty;
	}
	
	static ref array<EditorSelectableBase> SelectedObjects = {};
	static void ClearSelections()
	{
		foreach (EditorSelectableBase selected_object: SelectedObjects) {
			if (selected_object && selected_object.IsSelected()) {
				selected_object.SetSelected(false);
			}
		}
		
		SelectedObjects.Clear();
	}
}