
class EditorPlacedListItem: EditorListItem
{
	protected EditorObject m_EditorObject;
	EditorObject GetData() { 
		return m_EditorObject; 
	}
	
	void SetEditorObject(EditorObject data) 
	{ 
		EditorLog.Trace("EditorPlacedListItem::SetEditorObject"); 
		m_EditorObject = data;
		
		m_TemplateController.ListItemLabel = m_EditorObject.GetDisplayName();
		m_TemplateController.NotifyPropertyChanged("ListItemLabel");
		
		m_TemplateController.ListItemIcon = GetIconFromMod(m_EditorObject.GetData().ObjectMod);
		m_TemplateController.NotifyPropertyChanged("ListItemIcon");
				
		m_EditorObject.OnObjectSelected.Insert(EditorObjectSelected);
		m_EditorObject.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	
	void EditorObjectSelected(EditorObject data) {
		Select();
	}
	
	void EditorObjectDeselected(EditorObject data) {
		Deselect();
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		switch (args.GetMouseButton()) {
			
			case 0: {
				if (KeyState(KeyCode.KC_LCONTROL)) {
					GetEditor().ToggleSelection(GetData());
				} else if (KeyState(KeyCode.KC_LSHIFT)) {
					
					/*
					
					Select every list item between start selection and current click
					except i deleted ParentList ;)
					
					bool started_select;
					for (int i = 0; i < ParentList.Count(); i++) {
						EditorListItem list_item = ParentList[i];			
						if (started_select) {
							
							if (list_item.IsSelected() || list_item == this) {
								break;
							}
							
							GetEditor().SelectObject(EditorPlacedListItem.Cast(list_item).GetData());
							
						} else if (!started_select && (list_item.IsSelected() || list_item == this)) {
							started_select = true;
							continue;
						}
						
						
					}*/
					
				} else {
					GetEditor().ClearSelection();
					GetEditor().SelectObject(GetData());
				}
				break;
			} 
			
			case 1: {
				EditorContextMenu context_menu = new EditorContextMenu();
				int x, y;
				GetMousePos(x, y);
				context_menu.SetPosition(x, y);
				break;
			}
		}
		
		return true;
	}
	
	bool ListItemVisibleExecute(ButtonCommandArgs args)
	{
		switch (args.GetMouseButton()) {
			
			case 0: {
				m_EditorObject.ShowWorldObject(args.GetButtonState());
				break;
			}
			
		}
		
		return true;
	}
}
