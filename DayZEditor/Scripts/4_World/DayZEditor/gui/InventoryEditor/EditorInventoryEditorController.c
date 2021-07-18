class EditorInventoryEditorController: ViewController
{
	static const ref TStringArray RADIO_BUTTONS = {
		"ShoulderLeft",
		"ShoulderRight",
		"VestSlot",
		"ShirtSlot",
		"BeltSlot",
		"LegsSlot",
		"BackSlot",
		"HeadSlot",
		"FaceSlot",
		"EyeSlot",
		"GloveSlot",
		"FeetSlot",
		"ArmbandSlot"
	};
	
	PlayerBase Player;
	
	string SearchBarData;
	string SearchBarIcon = "set:dayz_editor_gui image:search";
	
	bool ShoulderLeft;
	bool ShoulderRight;
	bool VestSlot;
	bool ShirtSlot;
	bool BeltSlot;
	bool LegsSlot;
	bool BackSlot;
	bool HeadSlot;
	bool FaceSlot;
	bool EyeSlot;
	bool GloveSlot;
	bool FeetSlot;
	bool ArmbandSlot;
	
	ScrollWidget ItemSelectorScrollbar;
	
	ref ObservableCollection<ref EditorWearableListItem> WearableItems = new ObservableCollection<ref EditorWearableListItem>(this);
	ref map<string, ref array<ref EditorWearableItem>> LoadedWearableItems = new map<string, ref array<ref EditorWearableItem>>();
	
	void EditorInventoryEditorController()
	{		
		foreach (string button: RADIO_BUTTONS) {
			// Initialize the arrays!
			string slot = GetInventorySlot(button);
			LoadedWearableItems[slot] = new array<ref EditorWearableItem>();
		}
		
		EditorLog.Trace("EditorInventoryEditorController::LoadWearableObjects");
		g_Game.ReportProgress("Loading Wearable Objects");
		
		TStringArray config_paths = {};
		config_paths.Insert(CFG_VEHICLESPATH);
		config_paths.Insert(CFG_WEAPONSPATH);
		config_paths.Insert(CFG_MAGAZINESPATH);
		
		foreach (string path: config_paths) {
			for (int i = 0; i < GetGame().ConfigGetChildrenCount(path); i++) {
				string type;
				TStringArray inventory_slots = {};
		        GetGame().ConfigGetChildName(path, i, type);
				GetGame().ConfigGetTextArray(path + " " + type + " inventorySlot", inventory_slots);
				if (GetGame().ConfigGetInt(path + " " + type + " scope") < 2) { // maybe 2 is the way
					continue;
				}
				
				EditorWearableItem wearable_item = new EditorWearableItem(type, GetGame().ConfigGetTextOut(path + " " + type + " displayName"), inventory_slots);
				foreach (string inventory_slot: inventory_slots) {
					// Check if its a supported inventory slot
					if (LoadedWearableItems[inventory_slot]) {
						LoadedWearableItems[inventory_slot].Insert(wearable_item);
					}
				}

				/*if (!placeable_item || IsForbiddenItem(placeable_item.Type)) {
					continue;
				}*/
		    }
		}
		
		/*foreach (string name, array<ref EditorWearableItem> wearable_array: LoadedWearableItems) {
			Print(name);
			foreach (EditorWearableItem wearable_item: wearable_array) {
				Print("\t" + wearable_item.Type);
			}
		}*/
	}
	
	// playerSlots[] = {"Slot_Shoulder","Slot_Melee","Slot_Vest","Slot_Body","Slot_Hips","Slot_Legs","Slot_Back","Slot_Headgear","Slot_Mask","Slot_Eyewear","Slot_Gloves","Slot_Feet","Slot_Armband"};
	
	string GetInventorySlot(string radio_button)
	{
		switch (radio_button) {
			case "ShoulderLeft": return "Melee";
			case "ShoulderRight": return "Shoulder";
			case "VestSlot": return "Vest";
			case "ShirtSlot": return "Body";
			case "BeltSlot": return "Hips";
			case "LegsSlot": return "Legs";
			case "BackSlot": return "Back";
			case "HeadSlot": return "Headgear"; // Head does the funny face change
			case "FaceSlot": return "Mask";
			case "EyeSlot": return "Eyewear";
			case "GloveSlot": return "Gloves";
			case "FeetSlot": return "Feet";
			case "ArmbandSlot": return "Armband";
		}
		
		return string.Empty;
	}
		
	void OnListItemSelected(EditorWearableListItem list_item, EditorWearableItem wearable_item)
	{
		if (!Player) {
			return; // wat
		}
		
		int slot_id = InventorySlots.GetSlotIdFromString(list_item.GetSlot());
		
		// Clear existing item
		GetGame().ObjectDelete(Player.GetInventory().FindAttachment(slot_id));
		
		// Create new item on player
		Player.GetInventory().CreateAttachmentEx(wearable_item.Type, slot_id);
		
		// Deselect all other things
		for (int i = 0; i < WearableItems.Count(); i++) {
			WearableItems[i].SetSelected(WearableItems[i] == list_item);
		}
	}
	
	override void PropertyChanged(string property_name)
	{		
		// Radio Button Logic
		foreach (string button: RADIO_BUTTONS) {
			if (button == property_name) {
				// Should be enough?				
				WearableItems.Clear();
				
				string inventory_slot = GetInventorySlot(button);
				foreach (EditorWearableItem wearable: LoadedWearableItems[inventory_slot]) {
					// This is the part where we need to call NEW, not before
					EditorWearableListItem list_item = new EditorWearableListItem(wearable, inventory_slot);
					list_item.OnItemSelected.Insert(OnListItemSelected);
					WearableItems.Insert(list_item);
					
					// Assign active item from slot
					if (Player) {
						EntityAI slot_item = Player.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString(inventory_slot));
						if (slot_item && slot_item.GetType() == wearable.Type) {
							list_item.Select();
						}
					}
				}
									
				continue;
			}			
			
			// Reset scroll bar
			ItemSelectorScrollbar.VScrollToPos(0);
			
			EnScript.SetClassVar(this, button, 0, false);
			NotifyPropertyChanged(button, false);
		}
		
		switch (property_name) {
			
			case "SearchBarData": {
				for (int j = 0; j < WearableItems.Count(); j++) {
					WearableItems[j].GetLayoutRoot().Show(WearableItems[j].FilterType(SearchBarData)); 
				}
				
				ItemSelectorScrollbar.VScrollToPos(0);
				
				if (SearchBarData.Length() > 0) {
					SearchBarIcon = "set:dayz_gui image:icon_x";
				} else {
					SearchBarIcon = "set:dayz_editor_gui image:search";
				}
				
				NotifyPropertyChanged("SearchBarIcon");
				break;
			}			
		}
	}
}