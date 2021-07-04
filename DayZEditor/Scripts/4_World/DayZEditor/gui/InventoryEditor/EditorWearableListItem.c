class EditorWearableListItem: ScriptViewTemplate<EditorWearableListItemController>
{
	void EditorWearableListItem(EditorWearableItem wearable_item)
	{
		m_TemplateController.Type = wearable_item.Type;
		m_TemplateController.NotifyPropertyChanged("Type");
		
		m_TemplateController.DisplayName = wearable_item.DisplayName;
		m_TemplateController.NotifyPropertyChanged("DisplayName");
		
		m_TemplateController.Slots.Copy(wearable_item.Slots);
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