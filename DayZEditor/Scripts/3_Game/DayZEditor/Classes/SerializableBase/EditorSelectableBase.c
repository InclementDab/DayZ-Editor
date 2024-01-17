class EditorSelectableBase: SerializableBase
{
	ref ScriptInvoker OnSelectionChanged = new ScriptInvoker();
	
	protected bool m_IsSelected;
	protected string m_DisplayName;
	
	protected ref EditorTreeItem m_TreeItem = new EditorTreeItem(ScriptCaller.Create(SetSelected));
	
	void SetDisplayName(string display_name)
	{
		m_DisplayName = display_name;		
		m_TreeItem.SetText(m_DisplayName);
	}
	
	EditorTreeItem GetTreeItem()
	{		
		return m_TreeItem;
	}
	
	string GetDisplayName()
	{
		return m_DisplayName;
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