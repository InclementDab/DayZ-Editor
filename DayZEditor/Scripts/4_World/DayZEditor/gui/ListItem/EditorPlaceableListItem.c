
class EditorPlaceableListItem: EditorListItem
{
	protected ref EditorPlaceableItem m_PlaceableItem;
	
	void EditorPlaceableListItem(EditorPlaceableItem placeable_item)
	{
		m_PlaceableItem = placeable_item;
		
		ListItemShow.Show(false);
		m_TemplateController.Label = placeable_item.Type;
		m_TemplateController.NotifyPropertyChanged("Label");
		
		m_TemplateController.Icon = placeable_item.Icon;
		m_TemplateController.NotifyPropertyChanged("Icon");
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnStartPlacing.Insert(OnStartPlacing);
		EditorEvents.OnStopPlacing.Insert(OnStopPlacing);
#endif
	}
	
	override bool IsSelected() {
		
		if (GetEditor().ObjectInHand) {
			return (EditorHologram.Cast(GetEditor().ObjectInHand).GetPlaceableItem() == m_PlaceableItem);
		}
		
		return false;
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		switch (args.GetMouseButton()) {

			case 0: {
				GetEditor().CreateInHand(m_PlaceableItem);
				Select();
				break;
			}
		}
				
		return true;
	}
	
	void OnStartPlacing(Class context, EditorPlaceableItem placeable_item)
	{
		if (placeable_item == m_PlaceableItem) {
			Select();
		}
		else Deselect();
	}
	
	void OnStopPlacing(Class context)
	{
		Deselect();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorTooltip tooltip = new EditorTooltip();
		
		float size_x, size_y, pos_x, pos_y;
		m_LayoutRoot.GetScreenPos(pos_x, pos_y);
		m_LayoutRoot.GetScreenSize(size_x, size_y);
		
		tooltip.SetTitle(m_PlaceableItem.Type);
		
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		
		// bounds Y height to screen size
		// 150 is static height of tooltip
		if (pos_y + 300 > screen_y) {
			pos_y = screen_y - 300;
		}
		
		tooltip.GetLayoutRoot().SetPos(pos_x + size_x, pos_y);
		
		//! Animals and Zombies / Players "survivors"
		if (!GetGame().IsKindOf(m_PlaceableItem.Type, "Man") && !GetGame().IsKindOf(m_PlaceableItem.Type, "DZ_LightAI")) {
			tooltip.SetContent(GetGame().CreateObjectEx(m_PlaceableItem.Type, vector.Zero, ECE_NONE));
		}		
		
		EditorUIManager.SetCurrentTooltip(tooltip);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		EditorUIManager.SetCurrentTooltip(null);
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrag");	
		if (!GetEditor().IsPlacing()) {
			GetEditor().CreateInHand(m_PlaceableItem);
		}
		
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrop");
		
		if (GetEditor().IsPlacing()) {
			GetEditor().CommandManager.PlaceObjectCommand.Execute(this, null);
		}
		
		return true;
	}
		
	bool FilterType(BetterString filter)
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
}
