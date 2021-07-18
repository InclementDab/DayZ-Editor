class EditorInventoryAttachmentSlot: ScriptViewTemplate<EditorInventoryAttachmentSlotController>
{
	ref ScriptInvoker OnItemSelected = new ScriptInvoker();
	
	void EditorInventoryAttachmentSlot(string slot, string icon)
	{
		m_TemplateController.InventorySlot = slot;
		m_TemplateController.NotifyPropertyChanged("InventorySlot");
		
		m_TemplateController.Icon = icon;
		m_TemplateController.NotifyPropertyChanged("Icon");
	}
	
	void OnStateChanged(bool state)
	{
		if (state) {
			OnItemSelected.Invoke(this);
		}
	}
		
	string GetSlot()
	{	
		return m_TemplateController.InventorySlot;
	}
		
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/Inventory/InventoryButtons.layout";
	}
}