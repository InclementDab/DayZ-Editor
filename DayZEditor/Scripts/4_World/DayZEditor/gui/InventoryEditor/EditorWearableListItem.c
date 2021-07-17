class EditorWearableListItem: ScriptViewTemplate<EditorWearableListItemController>
{
	ref ScriptInvoker OnItemSelected = new ScriptInvoker();
	
	protected EditorWearableItem m_WearableItem;
	
	void EditorWearableListItem(EditorWearableItem wearable_item)
	{
		m_WearableItem = wearable_item;
		m_TemplateController.Type = wearable_item.Type;
		m_TemplateController.NotifyPropertyChanged("Type");
		
		m_TemplateController.DisplayName = wearable_item.DisplayName;
		m_TemplateController.NotifyPropertyChanged("DisplayName");
		
		m_TemplateController.Slots.Copy(wearable_item.Slots);
	}
	
	void ~EditorWearableListItem()
	{
		delete OnItemSelected;
	}
	
	void ListItemExecute(ButtonCommandArgs args)
	{
		OnItemSelected.Invoke(this, m_WearableItem);
	}
	
	string GetType()
	{
		return m_TemplateController.Type;
	}
	
	TStringArray GetInventorySlots()
	{
		return m_TemplateController.Slots;
	}
	
	bool IsValidInventorySlot(string slot)
	{
		return (m_TemplateController.Slots.Find(slot) != -1);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/Inventory/EditorWearableItem.layout";
	}
}