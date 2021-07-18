class EditorInventoryAttachmentSlotController: ViewController
{
	string InventorySlot;
	bool State;
	string Icon;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "State": {
				EditorInventoryAttachmentSlot.Cast(GetParent()).OnStateChanged(State);
				break;
			}
		}
	}
}