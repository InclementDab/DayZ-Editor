
class EditorPlacedListItem: EditorListItem
{
	protected ref EditorObject m_EditorObject;
	EditorObject GetData() { 
		return m_EditorObject; 
	}
	
	void SetEditorObject(EditorObject data) 
	{ 
		EditorLog.Trace("EditorPlacedListItem::SetEditorObject"); 
		m_EditorObject = data;
		
		GetListItemController().ListItemLabel = m_EditorObject.GetDisplayName();
		GetListItemController().NotifyPropertyChanged("ListItemLabel");
		
		GetListItemController().ListItemIcon = GetIconFromMod(m_EditorObject.GetData().ObjectMod);
		GetListItemController().NotifyPropertyChanged("ListItemIcon");
				
		m_EditorObject.OnObjectSelected.Insert(EditorObjectSelected);
		m_EditorObject.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	
	void EditorObjectSelected(EditorObject data) {
		Select();
	}
	
	void EditorObjectDeselected(EditorObject data) {
		Deselect();
	}

	override void ListItemExecute(ButtonCommandArgs args)
	{
		Select();
		if (args.GetMouseButton() == 0) {
			if (KeyState(KeyCode.KC_LCONTROL)) {
				GetEditor().ToggleSelection(GetData());
			} else {
				GetEditor().ClearSelection();
				GetEditor().SelectObject(GetData());
			}
		} else if (args.GetMouseButton() == 1) {
			EditorContextMenu context_menu = new EditorContextMenu();
			int x, y;
			GetMousePos(x, y);
			context_menu.SetPosition(x, y);
		}
	}
}
