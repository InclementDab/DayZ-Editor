class EditorWearableListItem: EditorListItem
{
	ref ScriptInvoker OnItemSelected = new ScriptInvoker();
	
	protected EditorWearableItem m_WearableItem;
	protected string m_InventorySlot;
	
	void EditorWearableListItem(EditorWearableItem wearable_item, string inventory_slot)
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
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		OnItemSelected.Invoke(this, m_WearableItem);
		return true;
	}
	
	string GetType()
	{
		return m_TemplateController.Label;
	}
	
	string GetSlot()
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
	
	override string GetLayoutFile()
	{
		return "DayZEditor/gui/Layouts/items/EditorWearableListItem.layout";
	}
}