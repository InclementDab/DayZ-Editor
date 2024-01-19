class EditorSelectableBase: SerializableBase
{
	static ref array<EditorSelectableBase> All = {};
	static ref array<EditorSelectableBase> SelectedObjects = {};
	static ref array<EditorSelectableBase> LockedObjects = {};
	
	ref ScriptInvoker OnSelectionChanged = new ScriptInvoker();
	ref ScriptInvoker OnLockChanged = new ScriptInvoker();
	
	protected bool m_IsSelected;
	protected bool m_IsLocked;
		
	protected ref EditorTreeItem m_TreeItem;
	
	void EditorSelectableBase()
	{
		All.Insert(this);
	}
	
	void ~EditorSelectableBase()
	{
		All.RemoveItem(this);
		
		delete m_TreeItem;
	}
		
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
				
		OnSelectionChanged.Invoke(this);
	}
	
	bool IsSelected() 
	{
		return m_IsSelected;
	}
	
	void SetLocked(bool locked)
	{
		m_IsLocked = locked;
		
		if (m_IsLocked) {
			LockedObjects.Insert(this);
		} else {
			LockedObjects.RemoveItem(this);
		}
				
		OnLockChanged.Invoke(this);
	}
	
	bool IsLocked()
	{
		return m_IsLocked;
	}
	
	string GetModel()
	{
		return string.Empty;
	}
	
	string GetName()
	{
		return string.Empty;
	}
	
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