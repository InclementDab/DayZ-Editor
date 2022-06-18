class EditorPlaceableListItem: EditorListItem
{
	//! Animals and Zombies / Players "survivors"
	static const ref TStringArray TOOLTIP_BLACKLIST = {
		"DZ_LightAI",
		"Man",
		"Car"
	};
	
	protected EditorPlaceableItem m_PlaceableItem;
	
	void EditorPlaceableListItem(EditorPlaceableItem placeable_item)
	{
		m_PlaceableItem = placeable_item;
		
		m_TemplateController.Label = placeable_item.Type;
		m_TemplateController.NotifyPropertyChanged("Label");
		
		m_TemplateController.Icon = placeable_item.Icon;
		m_TemplateController.NotifyPropertyChanged("Icon");
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnAddInHand.Insert(OnStartPlacing);
		EditorEvents.OnRemoveFromHand.Insert(OnStopPlacing);
#endif
		
		GetLayoutRoot().ClearFlags(WidgetFlags.DRAGGABLE);
	}
	
	override bool IsSelected() 
	{
		if (GetEditor().IsPlacing()) {
			array<EditorWorldObject> objects_in_hand = GetEditor().GetPlacingObjects();
			foreach (EditorWorldObject object_in_hand: objects_in_hand) {
				EditorHologram hologram = EditorHologram.Cast(object_in_hand);
				if (!hologram) {
					continue;
				}
				
				if (hologram.GetPlaceableItem() != m_PlaceableItem) {
					continue;
				}
				
				return true;
			}
		}
		
		return false;
	}
	
	EditorPlaceableItem GetPlaceableItem()
	{
		return m_PlaceableItem;
	}
		
	bool ListItemExecute(ButtonCommandArgs args)
	{
		switch (args.GetMouseButton()) {

			case 0: {
				GetEditor().ClearHand();
				GetEditor().AddInHand(m_PlaceableItem);
				Select();
				break;
			}
			
			case 1: {
				Select();
				int x, y;
				GetMousePos(x, y);
				EditorHud.CurrentMenu = new EditorPlaceableContextMenu(x, y, this);
				break;
			}
		}
				
		return true;
	}
	
	void OnStartPlacing(Class context, EditorWorldObject world_object, EditorHandData hand_data)
	{		
		EditorHologram hologram = EditorHologram.Cast(world_object);
		if (!hologram) {
			return;
		}
		
		if (hologram.GetPlaceableItem() != m_PlaceableItem) {
			Deselect();
			return;
		}
		
		Select();
	}
	
	void OnStopPlacing(Class context, EditorWorldObject world_object, EditorHandData hand_data)
	{
		Deselect();
	}
	
	override void Select()
	{
		super.Select();
		GetEditor().GetObjectManager().CurrentSelectedItem = m_PlaceableItem;
	}
	
	override void Deselect()
	{
		super.Deselect();
		//delete GetEditor().GetObjectManager().CurrentSelectedItem;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorPlaceableTooltip tooltip = new EditorPlaceableTooltip();
		
		float size_x, size_y, pos_x, pos_y;
		m_LayoutRoot.GetScreenPos(pos_x, pos_y);
		m_LayoutRoot.GetScreenSize(size_x, size_y);
		
		tooltip.SetTitle(m_PlaceableItem.Type);	
		tooltip.SetPosition(pos_x + size_x, pos_y);
		
		//! bugfix
		GetEditor().GetObjectManager().CurrentSelectedItem = m_PlaceableItem;
		
		if (m_PlaceableItem && !IsBlacklistedItem(m_PlaceableItem.Type)) {
			tooltip.SetContent(GetGame().CreateObjectEx(m_PlaceableItem.Type, Vector(0, -1000, 0), ECE_NONE));
		}		
		
		GetEditor().GetEditorHud().SetCurrentTooltip(tooltip);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		GetEditor().GetEditorHud().SetCurrentTooltip(null);
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrag");	
		if (!GetEditor().IsPlacing()) {
			GetEditor().AddInHand(m_PlaceableItem);
			w.Show(false);
		}
		
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrop");
		if (GetEditor().IsPlacing()) {
			GetEditor().CommandManager[EditorPlaceObjectCommand].Execute(this, null);
			w.Show(true);
		}
		
		return true;
	}
		
	override bool FilterType(string filter)
	{
		if (filter == string.Empty) return true;
		
		string type_lower = m_PlaceableItem.Type;
		type_lower.ToLower();
		filter.ToLower();
		
		if (filter[0] == "@") {
			type_lower = m_PlaceableItem.Mod.GetModName();
			filter[0] = "";
			type_lower.ToLower();
		}
		
		return type_lower.Contains(filter);
	}
	
	bool IsBlacklistedItem(string item_type)
	{
		foreach (string blacklist_check: TOOLTIP_BLACKLIST) {
			if (GetGame().IsKindOf(item_type, blacklist_check)) {
				return true;
			}
		}
		
		return false;
	}
	
	override bool EnableFavorites()
	{
		return true;
	}
	
	override bool OnFavoriteToggle(CheckBoxCommandArgs args)
	{
		array<string> favorite_items = {};
		GetGame().GetProfileStringList("EditorFavoriteItems", favorite_items);
		EditorLog.Debug("Toggling Favorite Favorite %1", m_PlaceableItem.Type);
		if (!m_TemplateController.Favorite) {
			favorite_items.Insert(m_PlaceableItem.Type);
		} else {
			// fixes a bug where several versions of an item were being placed in
			while (favorite_items.Find(m_PlaceableItem.Type) != -1) {
				favorite_items.Remove(favorite_items.Find(m_PlaceableItem.Type));					
			}
		}
		
		Print(favorite_items.Count());
		GetGame().SetProfileStringList("EditorFavoriteItems", favorite_items);
		GetGame().SaveProfile();
		return true;
	}
}
