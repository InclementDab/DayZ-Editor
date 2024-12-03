class EditorDeletedListItem: EditorListItem
{
	protected EditorDeletedObject m_EditorDeletedObject;
	
	void EditorDeletedListItem(EditorDeletedObject deleted_object)
	{
		m_EditorDeletedObject = deleted_object;
		
		string item_name = m_EditorDeletedObject.GetData().ModelName;		
		ListItemLabel.SetText(string.Format("%1 (%2)", item_name, m_EditorDeletedObject.GetID()));
		
		//m_TemplateController.Icon = m_EditorDeletedObject.GetData().Icon;
		//m_TemplateController.NotifyPropertyChanged("Icon");
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorDeletedListItem::ListItemExecute");
		switch (args.GetMouseButton()) {

			case MouseState.LEFT: {

				if (GetEditor().IsCtrlDown()) {
					GetEditor().ToggleHiddenObjectSelection(m_EditorDeletedObject);
					return true;
				} 
				
				if (!GetEditor().IsShiftDown()) {
					GetEditor().ClearSelection();
				}
				
				GetEditor().SelectHiddenObject(m_EditorDeletedObject);
				
				// Multi select handling
				if (GetEditor().IsShiftDown()) {
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
				vector pos = m_EditorDeletedObject.GetOriginalPosition();
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
		return m_EditorDeletedObject.IsSelected();
	}
	
	EditorDeletedObject GetDeletedObject()
	{
		return m_EditorDeletedObject;
	}
	
	override bool FilterType(string filter)
	{
		if (filter == string.Empty) return true;
		
		string type_lower = m_EditorDeletedObject.GetType();
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
		return "DayZEditor/gui/Layouts/items/EditorPlacedListItem.layout";
	}
}