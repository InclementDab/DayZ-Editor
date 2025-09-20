class EditorPlacedListItem: EditorListItem
{
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() 
	{ 
		return m_EditorObject; 
	}
			
	ImageWidget LockedImage, ToggleBoundingBoxImage, ToggleWorldMarkerImage;
	
	void EditorPlacedListItem(EditorObject editor_object)
	{
		EditorLog.Trace("EditorPlacedListItem::SetEditorObject"); 
		m_EditorObject = editor_object;
				
		m_TemplateController.Label = m_EditorObject.GetDisplayName();
		m_TemplateController.NotifyPropertyChanged("Label");
		
		m_TemplateController.Icon = m_EditorObject.GetData().Icon;
		m_TemplateController.NotifyPropertyChanged("Icon");
		
		LockedImage.Show(m_EditorObject.IsLocked());
		ToggleBoundingBoxImage.Show(m_EditorObject.GetFlags() & EditorObjectFlags.BBOX);
		ToggleWorldMarkerImage.Show(m_EditorObject.GetFlags() & EditorObjectFlags.OBJECTMARKER);
		ListItemLabel.SetText(editor_object.GetDisplayName());
		ListItemLabel.Show(true);
		
		m_EditorObject.OnObjectSelected.Insert(EditorObjectSelected);
		m_EditorObject.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
		
	void EditorObjectSelected(EditorObject data) 
	{
		Select();
	}
	
	void EditorObjectDeselected(EditorObject data) 
	{
		Deselect();
	}
	
	bool ListItemShowExecute(ButtonCommandArgs args)
	{
		m_EditorObject.Show(!args.GetButtonState());
		return true;
	}
	
	override bool IsSelected() 
	{
		return m_EditorObject.IsSelected();
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		switch (args.GetMouseButton()) {
			case MouseState.MIDDLE:
			case MouseState.LEFT: {
				if (args.GetMouseButton() == MouseState.MIDDLE) {
					vector clip_info[2];
					m_EditorObject.ClippingInfo(clip_info);
					
					vector transform[4];
					m_EditorObject.GetTransform(transform);
					
					vector high_point = 2 * Vector(Math.Max(clip_info[0][0], clip_info[1][0]), Math.Max(clip_info[0][1], clip_info[1][1]), Math.Max(clip_info[0][2], clip_info[1][2]));
					high_point[1] = (high_point[0] + high_point[2]) * 0.5;					
					vector new_position = high_point.Multiply4(transform);
					
					EditorCamera camera = GetEditor().GetCamera();
					camera.SetPosition(new_position);
					camera.LookAt(m_EditorObject.GetBottomCenter());
				}
				
				if (GetEditor().IsCtrlDown()) {
					GetEditor().ToggleSelection(m_EditorObject);
					return true;
				} 
				
				if (!GetEditor().IsShiftDown()) {
					GetEditor().ClearSelection();
				}
				
				GetEditor().SelectObject(m_EditorObject);
				
				// Multi select handling
				if (GetEditor().IsShiftDown()) {
					int this_index;
					int that_index = -1;
					EditorListItem tertiary_item;
					ObservableCollection<EditorListItem> list_items = GetEditor().GetEditorHud().GetTemplateController().RightbarPlacedData;					
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
					
					if (that_index == -1) {
						break;
					}
					
					GetEditor().GetEditorHud().GetTemplateController().DoMultiSelect(this_index, that_index, list_items);
				}
				
				return true;
			}
						
			case MouseState.RIGHT: {
				if (!m_EditorObject.IsSelected() && !GetEditor().IsShiftDown()) {
					GetEditor().ClearSelection();
				}
				
				GetEditor().SelectObject(m_EditorObject);
				
				if (EditorHud.CurrentMenu) {
					delete EditorHud.CurrentMenu;
				}
					
				int x, y;
				GetMousePos(x, y);
				EditorHud.CurrentMenu = new EditorPlacedContextMenu(x, y, m_EditorObject);
				
				return true;
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
	
	bool OnToggleLockExecute(ButtonCommandArgs args)
	{
		m_EditorObject.Lock(!m_EditorObject.IsLocked());
		if (m_EditorObject.IsLocked()) {
			ToggleBoundingBoxImage.Show(false);
			ToggleWorldMarkerImage.Show(false);
		} else {
			ToggleBoundingBoxImage.Show(m_EditorObject.GetFlags() & EditorObjectFlags.BBOX);
			ToggleWorldMarkerImage.Show(m_EditorObject.GetFlags() & EditorObjectFlags.OBJECTMARKER);
		}
		
		GetEditor().GetEditorHud().SetCurrentTooltip(null);
		
		return true;
	}
	
	void OnToggleBoundingBoxExecute(ButtonCommandArgs args)
	{
		bool new_state = !(m_EditorObject.GetFlags() & EditorObjectFlags.BBOX);
		m_EditorObject.SetBoundingBox(!m_EditorObject.IsBoundingBoxEnabled(), true);
		ToggleBoundingBoxImage.Show(new_state);
		
		GetEditor().GetEditorHud().SetCurrentTooltip(null);
	}	
	
	void OnWorldMarkerExecute(ButtonCommandArgs args)
	{
		bool new_state = !(m_EditorObject.GetFlags() & EditorObjectFlags.OBJECTMARKER);
		m_EditorObject.EnableObjectMarker(new_state);
		if (new_state) {
			m_EditorObject.GetData().Flags |= EditorObjectFlags.OBJECTMARKER;
		} else {
			m_EditorObject.GetData().Flags &= ~EditorObjectFlags.OBJECTMARKER;
		}
		
		ToggleWorldMarkerImage.Show(new_state);
		
		GetEditor().GetEditorHud().SetCurrentTooltip(null);
	}
		
	override bool OnDrag(Widget w, int x, int y)
	{
		GetEditor().SelectObject(m_EditorObject);
		array<EditorObject> additional_drag_targets = m_Editor.GetSelectedObjects().GetValueArray();
		additional_drag_targets.RemoveItem(m_EditorObject);
		
		GetEditor().DragHandler = new EditorObjectDragHandler();
		GetEditor().DragHandler.OnDragStart(m_EditorObject, additional_drag_targets);
		
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_EditorObject.GetMarker() && !m_EditorObject.GetMarker().IsSelected()) {
			m_EditorObject.GetMarker().SetHighlighted(1);
		}

		bool mouse_down = GetUApi().GetInputByID(UAMenuSelect).LocalValue();
		switch (w) {
			case LockedImage.GetParent(): {
				if (mouse_down) {
					m_EditorObject.Lock(!m_EditorObject.IsLocked());
					return true;
				}
				
				if (LockedImage.IsVisible() && m_EditorObject.IsLocked()) {					
					string command_name = GetEditor().CommandManager[EditorUnlockCommand].GetName();
					string command_shortcut = GetEditor().CommandManager[EditorUnlockCommand].GetShortcutString();
					GetDayZGame().DelaySetCurrentTooltip(EditorTooltip.CreateOnButton(command_name, w, TooltipPosition.BOTTOM_LEFT, string.Format("(%1)", command_shortcut)), w);
				} else {
					
					
					string command_name2 = GetEditor().CommandManager[EditorLockCommand].GetName();
					string command_shortcut2 = GetEditor().CommandManager[EditorLockCommand].GetShortcutString();
					GetDayZGame().DelaySetCurrentTooltip(EditorTooltip.CreateOnButton(command_name2, w, TooltipPosition.BOTTOM_LEFT, string.Format("(%1)", command_shortcut2)), w);
				}
				break;
			}
			
			case ToggleBoundingBoxImage.GetParent(): {
				// todo make toggle command
				//if (ToggleBoundingBoxImage.IsVisible()) {
					GetDayZGame().DelaySetCurrentTooltip(EditorTooltip.CreateOnButton("Toggle Bounding Box", w, TooltipPosition.BOTTOM_LEFT), w);
				//}
				
				if (mouse_down) {
					bool new_state = !(m_EditorObject.GetFlags() & EditorObjectFlags.BBOX);
					m_EditorObject.SetBoundingBox(!m_EditorObject.IsBoundingBoxEnabled(), true);
					ToggleBoundingBoxImage.Show(new_state);
					return true;
				}
				
				break;
			}
			
			case ToggleWorldMarkerImage.GetParent(): {
				//if (ToggleWorldMarkerImage.IsVisible()) {
					GetDayZGame().DelaySetCurrentTooltip(EditorTooltip.CreateOnButton("Toggle World Marker", w, TooltipPosition.BOTTOM_LEFT), w);
				//}
				
				if (mouse_down) {
					bool new_state2 = !(m_EditorObject.GetFlags() & EditorObjectFlags.OBJECTMARKER);
					m_EditorObject.EnableObjectMarker(new_state2);
					if (new_state2) {
						m_EditorObject.GetData().Flags |= EditorObjectFlags.OBJECTMARKER;
					} else {
						m_EditorObject.GetData().Flags &= ~EditorObjectFlags.OBJECTMARKER;
					}
					
					ToggleWorldMarkerImage.Show(new_state2);
					return true;
				}
				
				break;
			}
			
			default: {
				GetDayZGame().DelaySetCurrentTooltip(EditorTooltip.CreateOnButton(string.Format("(%1)", m_EditorObject.GetDisplayName()), w, TooltipPosition.BOTTOM_LEFT), w);
				break;
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_EditorObject.GetMarker() && !m_EditorObject.GetMarker().IsSelected()) {
			m_EditorObject.GetMarker().SetHighlighted(0);
		}
		
		GetEditor().GetEditorHud().ClearCurrentTooltip();
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (w.GetName() == "ListItemButton") {
			GetEditor().CommandManager[EditorObjectPropertiesCommand].Execute(this, null);
		}
		
		return true;
	}
	
	override bool FilterType(string filter)
	{
		if (filter == string.Empty) return true;
		
		string type_lower = m_EditorObject.GetType();
		type_lower.ToLower();
		filter.ToLower();
		
		if (filter[0] == "@") {
			type_lower = m_EditorObject.GetData().Mod.GetModName();
			filter[0] = "";
			type_lower.ToLower();
		}
		
		return type_lower.Contains(filter);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/gui/Layouts/items/EditorPlacedListItem.layout";
	}
}
