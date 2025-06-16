class EditorPlaceableListItemLarge: EditorPlaceableListItem
{
	protected EntityAI m_PreviewItem;

	ItemPreviewWidget ItemPreview;
	
	void EditorPlaceableListItemLarge(EditorPlaceableItem placeable_item)
	{
		Print(placeable_item.Type);
		Object preview_object = GetGame().CreateObjectEx(placeable_item.Type, vector.Zero, ECE_NONE);
		if (preview_object) {
			m_PreviewItem = EntityAI.Cast(preview_object);
			if (!m_PreviewItem) {
				preview_object.Delete();
			} else {
				ItemPreview.SetItem(m_PreviewItem);
				ItemPreview.SetView(0);
				ItemPreview.Update();
			}
		}
	}

	void ~EditorPlaceableListItemLarge()
	{
		if (m_PreviewItem) {
			m_PreviewItem.Delete();
		}
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/items/EditorListItemLarge.layout";
	}
}

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
		
		m_TemplateController.Label = placeable_item.GetName();
		m_TemplateController.NotifyPropertyChanged("Label");
				
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnAddInHand.Insert(OnStartPlacing);
		EditorEvents.OnRemoveFromHand.Insert(OnStopPlacing);
#endif
		
		if (GetEditor().GetSettings().FavoriteItems.Find(m_PlaceableItem.Type) != -1) {
			m_TemplateController.Favorite = true;
			ListItemFavorites.Show(true);
			m_TemplateController.NotifyPropertyChanged("Favorite");
		}
		
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
		
		tooltip.SetPosition(pos_x + size_x + 7, pos_y);
		
		//! bugfix
		GetEditor().GetObjectManager().CurrentSelectedItem = m_PlaceableItem;
		
		bool preview_disabled = GetGame().IsKindOf(m_PlaceableItem.Type, "Man") || GetGame().IsKindOf(m_PlaceableItem.Type, "DZ_LightAI");
		if (m_PlaceableItem && !preview_disabled) {
			Object preview = GetGame().CreateObjectEx(m_PlaceableItem.Type, Vector(0, -1000, 0), ECE_NONE);
			if (!preview) {
				// DOESNT WORK @JACOB
				string new_type = GetEditor().GetObjectManager().ConvertP3dFileToPotentialObjectType(m_PlaceableItem.Type);
				if (new_type) {
					preview = GetGame().CreateObjectEx(new_type, Vector(0, -1000, 0), ECE_NONE);
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
		
		return super.OnDrag(w, x, y);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrop");
		if (GetEditor().IsPlacing()) {
			GetEditor().CommandManager[EditorPlaceObjectCommand].Execute(this, null);
			w.Show(true);
		}
		
		return super.OnDrop(w, x, y, reciever);
	}
		
	override bool FilterType(string filter)
	{
		if (filter == string.Empty) return true;
		
		string type_lower = m_PlaceableItem.Type;
		type_lower.ToLower();
		filter.ToLower();
				
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
		EditorLog.Debug("Toggling Favorite Favorite %1", m_PlaceableItem.Type);

		EditorSettings settings = GetEditor().GetSettings();
		if (!args.GetCheckBoxState()) {
			int index = settings.FavoriteItems.Find(m_PlaceableItem.Type);
			if (index != -1) {
				settings.FavoriteItems.Remove(index);
				settings.Save();
			}
		} else {
			if (settings.FavoriteItems.Find(m_PlaceableItem.Type) == -1) {
				settings.FavoriteItems.Insert(m_PlaceableItem.Type);
				settings.Save();
			}
		}

		return true;
	}
}
