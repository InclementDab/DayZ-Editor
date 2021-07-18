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
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorHud.SetCurrentTooltip(EditorTooltip.CreateOnButton(m_TemplateController.InventorySlot, w, TooltipPositions.BOTTOM_RIGHT));
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		EditorHud.SetCurrentTooltip(null);
		return true;
	}
		
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/Inventory/InventoryButtons.layout";
	}
}