class EditorPlaceableListNode: EditorListNode
{
	protected EditorPlaceableItem m_PlaceableItem;
	protected string m_SearchString1, m_SearchString2;
	
	void EditorPlaceableListNode(notnull EditorPlaceableItem placeable_item)
	{
		m_PlaceableItem = placeable_item;
		m_SearchString1 = m_PlaceableItem.Type;
		m_SearchString2 = m_PlaceableItem.Name;
		
		m_SearchString1.ToLower();
		m_SearchString2.ToLower();
		
		Text.SetText(m_PlaceableItem.Name);		
		if (placeable_item.Type.Contains(".p3d")) {
			Symbols.TREE_DECIDUOUS.Load(IconImage, 2);
			IconImage.SetColor(LinearColor.LIGHT_YELLOW);
			m_LayoutRoot.SetSort(100);
		} else if (placeable_item.Type.Contains("_DE")) {
			Symbols.MONEY_BILL.Load(IconImage, 2);
			IconImage.SetColor(LinearColor.LIGHT_BLUE);
		} else {
			if (GetGame().IsKindOf(placeable_item.Type, "Inventory_Base")) {
				Symbols.SHOVEL.Load(IconImage, 2);
				IconImage.SetColor(LinearColor.LIGHT_BLUE);
			} else {
				Symbols.HOUSE_BLANK.Load(IconImage, 2);
				IconImage.SetColor(LinearColor.LIGHT_BLUE);
			}
		}
		
		Favorite.Show(true);
		
		if (GetEditor().GetSettings().FavoriteItems.Find(placeable_item.Type) != -1) {
			FavoriteIcon.SetImage(3);
			FavoriteIcon.SetColor(LinearColor.GOLD);
		} else {
			FavoriteIcon.SetImage(2);
			FavoriteIcon.SetColor(LinearColor.WHITE);
		}
		
		EditorEvents.OnObjectPlaced.Insert(OnObjectPlaced);
	}
	
	protected void OnObjectPlaced(Class context, EditorObject target)
	{
		if (target && target.GetType() == m_PlaceableItem.Type) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(OnMouseLeave, 0, 0, m_LayoutRoot, null, 0, 0);
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
			Object preview = GetGame().CreateObjectEx(m_PlaceableItem.Type, Vector(0, -1000, 0), ECE_LOCAL | ECE_INITAI);
			if (!preview) {
				// DOESNT WORK @JACOB
				string new_type = GetEditor().GetObjectManager().ConvertP3dFileToPotentialObjectType(m_PlaceableItem.Type);
				if (new_type) {
					preview = GetGame().CreateObjectEx(new_type, Vector(0, -1000, 0), ECE_LOCAL | ECE_INITAI);
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
		return super.OnMouseLeave(w, enterW, x, y);
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
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w) {
			case FavoriteButton: {
				bool is_favorite = GetEditor().GetSettings().FavoriteItems.Find(m_PlaceableItem.Type) != -1;
				if (is_favorite) {
					GetEditor().GetSettings().FavoriteItems.RemoveItem(m_PlaceableItem.Type);
					FavoriteIcon.SetImage(2);
					FavoriteIcon.SetColor(LinearColor.WHITE);
					GetEditor().GetEditorHud().RefreshSearchBar();
				} else {
					GetEditor().GetSettings().FavoriteItems.Insert(m_PlaceableItem.Type);
					FavoriteIcon.SetImage(3);
					FavoriteIcon.SetColor(LinearColor.GOLD);
				}
				
				GetEditor().GetSettings().Save();
				break;
			}
		}
			
		return super.OnClick(w, x, y, button);
	}
	
	override bool FilterType(string filter, bool favorites)
	{
		if (!filter && !favorites) {
			return true;
		}
		
		bool matches_filter = (m_SearchString1.Contains(filter) || m_SearchString2.Contains(filter) || !filter);
		if (favorites) {
			return (matches_filter && GetEditor().GetSettings().FavoriteItems.Find(m_PlaceableItem.Type) != -1);
		}
		
		return matches_filter;
	}
	
	override bool IsSelected()
	{
		auto placing_objects = GetEditor().GetPlacingObjects();
		foreach (auto placing_object: placing_objects) {
			
			EditorHologram hologram = EditorHologram.Cast(placing_object);
			if (hologram && hologram.GetPlaceableItem() == m_PlaceableItem) {
				return true;
			}
		}
		
		return false;
	}
}