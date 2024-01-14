class EditorDeletedListItem: EditorListItem
{
	protected EditorHiddenObject m_EditorHiddenObject;
	
	void EditorDeletedListItem(EditorHiddenObject deleted_object)
	{
		m_EditorHiddenObject = deleted_object;
		
		string item_name = m_EditorHiddenObject.GetType();
		if (item_name.Length() >= 30) {
			item_name = item_name.Substring(0, 28) + "...";
		}
		
		m_TemplateController.Label = string.Format("%1 (%2)", item_name, m_EditorHiddenObject.GetID());
		m_TemplateController.NotifyPropertyChanged("Label");
		
		//m_TemplateController.Icon = m_EditorHiddenObject.GetData().Icon;
		//m_TemplateController.NotifyPropertyChanged("Icon");
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorDeletedListItem::ListItemExecute");
		switch (args.GetMouseButton()) {

			case MouseState.LEFT: {

				if (KeyState(KeyCode.KC_LCONTROL)) {
					GetEditor().ToggleHiddenObjectSelection(m_EditorHiddenObject);
					return true;
				} 
				
				if (!KeyState(KeyCode.KC_LSHIFT)) {
					GetEditor().ClearSelection();
				}
				
				GetEditor().SelectHiddenObject(m_EditorHiddenObject);
				
				// Multi select handling
				if (KeyState(KeyCode.KC_LSHIFT)) {
					int this_index, that_index;
					EditorListItem tertiary_item;
					ObservableCollection<EditorListItem> list_items = GetEditor().GetEditorHud().GetTemplateController().RightbarDeletionData;
					for (int i = 0; i <= list_items.Count(); i++) {
						if (list_items[i] == this) {
							this_index = i;
							continue;
						}
						
						if (list_items[i] && list_items[i].IsSelected()) {
							that_index = i;
							continue;
						}
					}
						
					GetEditor().GetEditorHud().GetTemplateController().DoMultiSelect(this_index, that_index, list_items);
				}
				
				return true;
			}
			
			case MouseState.MIDDLE: {
				EditorCamera camera = GetEditor().GetCamera();
				vector pos = m_EditorHiddenObject.GetOriginalPosition();
				pos[1] = camera.GetPosition()[1];
				camera.SetPosition(pos);
				return true;
			}			
		}
		
		return true;
	}
	
	override void Select()
	{
		ListItem.SetColor(LIST_ITEM_COLOR_ON_DELETED);
	}
	
	override bool IsSelected()
	{
		return m_EditorHiddenObject.IsSelected();
	}
	
	EditorHiddenObject GetDeletedObject()
	{
		return m_EditorHiddenObject;
	}
	
	override bool FilterType(string filter)
	{
		if (filter == string.Empty) return true;
		
		string type_lower = m_EditorHiddenObject.GetType();
		type_lower.ToLower();
		filter.ToLower();
		
		/*
		if (filter[0] == "@") {
			type_lower = m_EditorObject.GetData().Mod.GetModName();
			filter[0] = "";
			type_lower.ToLower();
		}*/
		
		return type_lower.Contains(filter);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/items/EditorDeletionsListItem.layout";
	}
}