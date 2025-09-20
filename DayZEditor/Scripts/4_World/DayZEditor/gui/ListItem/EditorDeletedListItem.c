class EditorDeletedListItem: EditorListItem
{
	protected EditorDeletedObject m_EditorDeletedObject;
	Widget WrapSpacerWidget1;
	
	void EditorDeletedListItem(EditorDeletedObject deleted_object)
	{
		m_EditorDeletedObject = deleted_object;
		
		string item_name = m_EditorDeletedObject.GetData().ModelName;		
		ListItemLabel.SetText(string.Format("%1 (%2)", item_name, m_EditorDeletedObject.GetID()));
		
		WrapSpacerWidget1.Show(false);
		//m_TemplateController.Icon = m_EditorDeletedObject.GetData().Icon;
		//m_TemplateController.NotifyPropertyChanged("Icon");
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorDeletedListItem::ListItemExecute");
		switch (args.GetMouseButton()) {
			case MouseState.MIDDLE:
			case MouseState.LEFT: {
				if (args.GetMouseButton() == MouseState.MIDDLE) {
					vector clip_info[2];
					m_EditorDeletedObject.ClippingInfo(clip_info);
					
					vector transform[4];
					m_EditorDeletedObject.GetTransform(transform);
					
					vector high_point = 2 * Vector(Math.Max(clip_info[0][0], clip_info[1][0]), Math.Max(clip_info[0][1], clip_info[1][1]), Math.Max(clip_info[0][2], clip_info[1][2]));
					high_point[1] = (high_point[0] + high_point[2]) * 0.5;					
					vector new_position = high_point.Multiply4(transform);
					
					EditorCamera camera = GetEditor().GetCamera();
					camera.SetPosition(new_position);
					camera.LookAt(m_EditorDeletedObject.GetOriginalPosition());
				}			

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