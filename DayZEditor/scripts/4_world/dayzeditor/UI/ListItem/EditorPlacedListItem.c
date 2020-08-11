class EditorPlacedListItem: EditorListItem
{
	private ref EditorObject m_Data;
	EditorObject GetData() { return m_Data; }
	
	void EditorPlacedListItem(EditorObject data)
	{
		Print("EditorPlacedListItem");
		m_Data = data;
		
		m_ListItemCollapse.Show(false);
		
		SetText(m_Data.GetType());
		SetIcon(Editor.GetIconFromMod(Editor.GetModFromObject(m_Data.GetType())));
		
		m_Data.OnObjectSelected.Insert(EditorObjectSelected);
		m_Data.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	void EditorObjectSelected(EditorObject data)
	{
		SetColor(COLOR_ON_SELECTED);
	}
	
	void EditorObjectDeselected(EditorObject data)
	{
		SetColor(COLOR_ON_DESELECTED);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == 0) {
		
			if (GetGame().GetInput().LocalValue("UAWalkRunTemp"))
				GetEditor().GetObjectManager().ToggleSelection(m_Data);
			else {
				EditorEvents.SelectObject(this, m_Data);
			}
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	
}
