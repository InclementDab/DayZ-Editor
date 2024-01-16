class EditorInventoryAttachmentSlotController: ViewController
{
	int SlotId;
	string Icon;

	bool State;
	
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