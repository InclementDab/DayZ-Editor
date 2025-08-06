class EditorPlacedListNode: EditorListNode
{
	protected EditorObject m_EditorObject;
	
	void EditorPlacedListNode(notnull EditorObject editor_object)
	{
		m_EditorObject = editor_object;
		Text.SetText(editor_object.GetDisplayName());
		Hide.Show(true);
		BoundingBox.Show(true);
		Lock.Show(true);
		Marker.Show(true);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w) {
			case HideButton: {
				if (HideButton.GetState()) {
					Symbols.EYE_SLASH.Load(HideIcon);
				} else {
					Symbols.EYE.Load(HideIcon);
				}
				
				return true;
			}
		}
		
		switch (button) {
			case MouseState.LEFT: {
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
					/*
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
					
					GetEditor().GetEditorHud().GetTemplateController().DoMultiSelect(this_index, that_index, list_items);*/
					
				}
				
				return true;
			}
			
			case MouseState.MIDDLE: {
				EditorCamera camera = GetEditor().GetCamera();

				vector camera_transform[4];
				camera.GetTransform(camera_transform);
				Math3D.MatrixInverse3(camera_transform);
				camera_transform[3] = m_EditorObject.GetPosition();

				vector new_position = Vector(4.0, 4.0, 4.0).Multiply4(camera_transform);
				camera.SetPosition(new_position);
				camera.LookAt(m_EditorObject.GetPosition());
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
					
				EditorHud.CurrentMenu = new EditorPlacedContextMenu(x, y, m_EditorObject);
				
				return true;
			}
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		GetEditor().SelectObject(m_EditorObject);
		array<EditorObject> additional_drag_targets = GetEditor().GetSelectedObjects().GetValueArray();
		additional_drag_targets.RemoveItem(m_EditorObject);
		
		GetEditor().DragHandler = new EditorObjectDragHandler();
		GetEditor().DragHandler.OnDragStart(m_EditorObject, additional_drag_targets);
		
		return true;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		GetEditor().CommandManager[EditorObjectPropertiesCommand].Execute(this, null);
		
		return true;
	}
	
	protected override bool IsSelected()
	{
		return m_EditorObject.IsSelected();
	}
}