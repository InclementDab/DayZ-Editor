
class EditorPlacedListItem: EditorListItem
{
	private ref EditorObject m_Data;
	EditorObject GetData() { 
		return m_Data; 
	}
	
	void EditorPlacedListItem(EditorObject data) 
	{ 
		EditorLog.Trace("EditorPlacedListItem"); 
		m_Data = data;
		
		m_Controller.SetLabel(m_Data.GetDisplayName());
		m_Controller.SetIcon(GetIconFromMod(m_Data.GetData().ObjectMod));
		
		m_Data.OnObjectSelected.Insert(EditorObjectSelected);
		m_Data.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	
	void EditorObjectSelected(EditorObject data) {
		m_Controller.Select();
	}
	
	void EditorObjectDeselected(EditorObject data) {
		m_Controller.Deselect();
	}

}
