
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
		
		m_EditorListItemController.EditorListItemCollapse.Show(false);
		m_EditorListItemController.EditorListItemLabelData = m_Data.GetDisplayName();
		m_EditorListItemController.EditorListItemIconData = GetIconFromMod(m_Data.GetData().ObjectMod);
		
		m_Data.OnObjectSelected.Insert(EditorObjectSelected);
		m_Data.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	
	void EditorObjectSelected(EditorObject data) {
		m_EditorListItemController.Select();
	}
	
	void EditorObjectDeselected(EditorObject data) {
		m_EditorListItemController.Deselect();
	}
	/*
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == 0) {
		
			if (GetGame().GetInput().LocalValue("UAWalkRunTemp"))
				GetEditor().ToggleSelection(m_Data);
			else {
				GetEditor().SelectObject(m_Data);
			}
		}
		
		return super.OnClick(w, x, y, button);
	}
	*/
	
}
