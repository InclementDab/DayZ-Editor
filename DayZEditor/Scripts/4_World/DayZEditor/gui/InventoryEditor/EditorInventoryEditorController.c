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
	
	ref ObservableCollection<EditorWearableListItem> WearableItems = new ObservableCollection<EditorWearableListItem>(this);
	ref map<string, ref array<ref EditorWearableItem>> LoadedWearableItems = new map<string, ref array<ref EditorWearableItem>>();
	
	void EditorInventoryEditorController()
	{
		foreach (string button: RADIO_BUTTONS) {
			// Initialize the arrays!
			string slot = GetInventorySlot(button);
			LoadedWearableItems[slot] = new array<ref EditorWearableItem>();
		}
		
		EditorLog.Trace("EditorInventoryEditorHud::LoadPlaceableObjects");
		g_Game.ReportProgress("Loading Wearable Objects");
		
		//array<ref EditorWearableListItem> wearable_items();
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
				if (GetGame().ConfigGetInt(path + " " + type + " scope") == 0) {
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
	
	string GetInventorySlot(string radio_button)
	{
		switch (radio_button) {
			case "ShoulderLeft": return "Shoulder";
			case "ShoulderRight": return "Shoulder";
			case "VestSlot": return "Vest";
			case "ShirtSlot": return "Body";
			case "BeltSlot": return "Hips";
			case "LegsSlot": return "Legs";
			case "BackSlot": return "Back";
			case "HeadSlot": return "Head";
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
		Print(wearable_item.Type);
		Print(wearable_item.Slots);
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
					EditorWearableListItem list_item = new EditorWearableListItem(wearable);
					list_item.OnItemSelected.Insert(OnListItemSelected);
					WearableItems.Insert(list_item);
				}
				
				continue;
			}
			
			EnScript.SetClassVar(this, button, 0, false);
			NotifyPropertyChanged(button, false);
		}
	}
}