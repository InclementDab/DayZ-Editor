
class EditorPlaceableListItem: EditorListItem
{
	protected ref EditorPlaceableItem m_PlaceableItem;
	
	void EditorPlaceableListItem(EditorPlaceableItem placeable_item)
	{
		m_PlaceableItem = placeable_item;
		
		m_TemplateController.Label = placeable_item.Type;
		m_TemplateController.NotifyPropertyChanged("Label");
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnStartPlacing.Insert(OnStartPlacing);
		EditorEvents.OnStopPlacing.Insert(OnStopPlacing);
#endif
	}
	
	override bool IsSelected() {
		
		if (GetEditor().ObjectInHand) {
			return (GetEditor().ObjectInHand.GetPlaceableItem() == m_PlaceableItem);
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
			
			case 1: {
				int x, y;
				GetMousePos(x, y);
				EditorPlaceableContextMenu placeable_context = new EditorPlaceableContextMenu(x, y);
				EditorUIManager.CurrentMenu = placeable_context;
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
		tooltip.GetLayoutRoot().SetPos(pos_x + size_x, pos_y);
		tooltip.SetContent(GetWorkbenchGame().CreateObjectEx(m_PlaceableItem.Type, vector.Zero, ECE_NONE));
		
		EditorUIManager.SetCurrentTooltip(tooltip);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		EditorUIManager.SetCurrentTooltip(null);
		
		return super.OnMouseLeave(w, enterW, x, y);
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

/*
class EditorPlaceableListItem: EditorListItem
{
	
	void EditorPlaceableListItem(Widget parent, string type, string path)
	{
		EditorLog.Trace("EditorPlaceableListItem");
		
		m_TemplateController.ListItemLabel = Type;
		m_TemplateController.NotifyPropertyChanged("ListItemLabel");
		
		if (m_ModStructure) {
			m_TemplateController.ListItemIcon = GetIconFromMod(m_ModStructure);
			m_TemplateController.NotifyPropertyChanged("ListItemIcon");
		} else {
			EditorLog.Debug("Mod not found for %1", Type);
		}
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnStartPlacing.Insert(StartPlacing);
		EditorEvents.OnStopPlacing.Insert(StopPlacing);
#endif
		
	}
	
	bool FilterType(BetterString filter)
	{
		if (filter == string.Empty) return true;
		
		string type_lower = Type;
		type_lower.ToLower();
		filter.ToLower();
		
		if (filter[0] == "@") {
			type_lower = m_ModStructure.GetModName();
			filter[0] = "";
			type_lower.ToLower();
		}
		
		return type_lower.Contains(filter);
	}


	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrag");	
		if (!GetEditor().IsPlacing()) {
			GetEditor().CreateInHand(this);
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
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorTooltip tooltip = new EditorTooltip();
		
		float size_x, size_y, pos_x, pos_y;
		m_LayoutRoot.GetScreenPos(pos_x, pos_y);
		m_LayoutRoot.GetScreenSize(size_x, size_y);
		
		tooltip.SetTitle(Type);
		tooltip.GetLayoutRoot().SetPos(pos_x + size_x, pos_y);
		tooltip.SetContent(GetWorkbenchGame().CreateObjectEx(Type, vector.Zero, ECE_NONE));
		
		if (EditorUIManager.CurrentTooltip) {
			delete EditorUIManager.CurrentTooltip;
		}
		
		EditorUIManager.CurrentTooltip = tooltip;
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (EditorUIManager.CurrentTooltip) {
			delete EditorUIManager.CurrentTooltip;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorPlaceableListItem::ListItemExecute");
		
		if (args.GetMouseButton() == 0 && !GetEditor().IsPlacing()) {
			GetEditor().CreateInHand(this);
		} 
		
		else if (args.GetMouseButton() == 1) {
			EditorPlaceableContextMenu placeable_context = new EditorPlaceableContextMenu();
			int x, y;
			GetMousePos(x, y);
			placeable_context.SetPosition(x, y);
		}
		
		return true;
	}
}