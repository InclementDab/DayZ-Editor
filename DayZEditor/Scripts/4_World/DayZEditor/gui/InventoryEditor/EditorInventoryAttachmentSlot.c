class EditorInventoryAttachmentSlot: ScriptViewTemplate<EditorInventoryAttachmentSlotController>
{
	ref ScriptInvoker OnItemSelected = new ScriptInvoker();
	
	void EditorInventoryAttachmentSlot(int slot)
	{
		m_TemplateController.SlotId = slot;
		m_TemplateController.NotifyPropertyChanged("SlotId");
		
		m_TemplateController.Icon = GetSlotImageFromSlotName(InventorySlots.GetSlotName(slot));
		m_TemplateController.NotifyPropertyChanged("Icon");
	}
	
	void OnStateChanged(bool state)
	{
		if (state) {
			OnItemSelected.Invoke(this);
		}
	}
		
	int GetSlot()
	{	
		return m_TemplateController.SlotId;
	}
	
	static string GetSlotDisplayName(int slot_id)
	{
		string name = InventorySlots.GetSlotDisplayName(slot_id);
		if (name == string.Empty) {
			name = InventorySlots.GetSlotName(slot_id);
		}
		
		return name;
	}
	
	static string GetSlotImageFromSlotName(string slot_name)
	{
		if (!GetGame().ConfigIsExisting(string.Format("CfgSlots Slot_%1 ghostIcon", slot_name))) {
			return "set:dayz_inventory image:missing";
		}
		
		// crackhead shit
		return GetGame().ConfigGetTextOut(string.Format("CfgSlots Slot_%1 ghostIcon", slot_name));
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorHud.SetCurrentTooltip(EditorTooltip.CreateOnButton(GetSlotDisplayName(m_TemplateController.SlotId), w, TooltipPositions.BOTTOM_RIGHT));
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