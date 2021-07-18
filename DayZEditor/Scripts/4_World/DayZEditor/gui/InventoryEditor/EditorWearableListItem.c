class EditorWearableListItem: EditorListItem
{
	ref ScriptInvoker OnItemSelected = new ScriptInvoker();
	
	protected EditorWearableItem m_WearableItem;
	protected int m_InventorySlot;
	
	protected bool m_Selected;
	
	void EditorWearableListItem(EditorWearableItem wearable_item, int inventory_slot)
	{
		//EditorLog.Trace("EditorWearableListItem");
		m_WearableItem = wearable_item;
		m_InventorySlot = inventory_slot;
		m_TemplateController.Label = m_WearableItem.Type;
		m_TemplateController.NotifyPropertyChanged("Label");
		
		//m_TemplateController.DisplayName = wearable_item.DisplayName;
		//m_TemplateController.NotifyPropertyChanged("DisplayName");
	}
	
	void ~EditorWearableListItem()
	{
		//EditorLog.Trace("~EditorWearableListItem");
		delete OnItemSelected;
	}
	
	void SetSelected(bool state)
	{
		if (state) Select();
		else Deselect();
	}
	
	// Locally storing for now
	override void Select()
	{
		m_Selected = true;
		super.Select();
	}
	
	override void Deselect()
	{
		m_Selected = false;
		super.Deselect();
	}
	
	override bool IsSelected()
	{
		return m_Selected;
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		OnItemSelected.Invoke(this, m_WearableItem);
		return true;
	}
	
	string GetType()
	{
		return m_TemplateController.Label;
	}
	
	int GetSlot()
	{
		return m_InventorySlot;
	}
	
	TStringArray GetInventorySlots()
	{
		return m_WearableItem.Slots;
	}
	
	EditorWearableItem GetWearableItem()
	{
		return m_WearableItem;
	}
	
	bool IsValidInventorySlot(string slot)
	{
		return (m_WearableItem.Slots.Find(slot) != -1);
	}
	
	bool FilterType(string filter)
	{
		if (filter == string.Empty) return true;
		
		string type_lower = m_WearableItem.Type;
		type_lower.ToLower();
		filter.ToLower();
		
		/*if (filter[0] == "@") {
			type_lower = m_PlaceableItem.Mod.GetModName();
			filter[0] = "";
			type_lower.ToLower();
		}*/
		
		return type_lower.Contains(filter);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/gui/Layouts/items/EditorWearableListItem.layout";
	}
}