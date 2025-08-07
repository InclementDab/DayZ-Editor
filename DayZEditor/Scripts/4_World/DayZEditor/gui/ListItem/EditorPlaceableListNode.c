class EditorPlaceableListNode: EditorListNode
{
	protected EditorPlaceableItem m_PlaceableItem;
	
	void EditorPlaceableListNode(notnull EditorPlaceableItem placeable_item)
	{
		m_PlaceableItem = placeable_item;
		
		Text.SetText(m_PlaceableItem.Name);		
		if (placeable_item.Type.Contains(".p3d")) {
			Symbols.TREE_DECIDUOUS.Load(IconImage, 3);
			m_LayoutRoot.SetSort(100);
		} else {
			Symbols.HOUSE_BLANK.Load(IconImage, 3);
		}
	}
				
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorPlaceableTooltip tooltip = new EditorPlaceableTooltip();
		
		float size_x, size_y, pos_x, pos_y;
		GetEditor().GetEditorHud().LeftbarScroll.GetScreenSize(size_x, size_y);
		tooltip.SetPosition(size_x + 12, y);
		
		//! bugfix
		GetEditor().GetObjectManager().CurrentSelectedItem = m_PlaceableItem;
		
		bool preview_disabled = GetGame().IsKindOf(m_PlaceableItem.Type, "Man") || GetGame().IsKindOf(m_PlaceableItem.Type, "DZ_LightAI");
		if (m_PlaceableItem && !preview_disabled) {
			Object preview = GetGame().CreateObjectEx(m_PlaceableItem.Type, Vector(0, -1000, 0), ECE_LOCAL);
			if (!preview) {
				// DOESNT WORK @JACOB
				string new_type = GetEditor().GetObjectManager().ConvertP3dFileToPotentialObjectType(m_PlaceableItem.Type);
				if (new_type) {
					preview = GetGame().CreateObjectEx(new_type, Vector(0, -1000, 0), ECE_LOCAL);
				}
			}

			string display_name = string.Empty;
			if (preview) {
				display_name = preview.GetDisplayName();
			}
				
			if (!display_name) {
				display_name = m_PlaceableItem.Type;
			}

			tooltip.SetTitle(display_name);	

			if (preview) {
				tooltip.SetContent(preview);
			}
		}		
		
		GetEditor().GetEditorHud().SetCurrentTooltip(tooltip);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		GetEditor().GetEditorHud().ClearCurrentTooltip();
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		GetEditor().GetObjectManager().CurrentSelectedItem = m_PlaceableItem;
		switch (button) {
			case 0: {
				GetEditor().ClearHand();
				GetEditor().AddInHand(m_PlaceableItem);
				break;
			}
			
			case 1: {
				EditorHud.CurrentMenu = new EditorPlaceableContextMenu(x, y, m_PlaceableItem);
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		if (!GetEditor().IsPlacing()) {
			GetEditor().AddInHand(m_PlaceableItem);
			w.Show(false);
		}
		
		return super.OnDrag(w, x, y);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		if (GetEditor().IsPlacing()) {
			GetEditor().CommandManager[EditorPlaceObjectCommand].Execute(this, null);
			w.Show(true);
		}
		
		return super.OnDrop(w, x, y, reciever);
	}
}