class EditorWearableListItemController: ViewController
{
	string Type;
	ref TStringArray Slots = {};
}

class EditorWearableListItem: ScriptViewTemplate<EditorWearableListItemController>
{
	void EditorWearableListItem(string type, TStringArray inventory_slots)
	{
		m_TemplateController.Type = type;
		m_TemplateController.NotifyPropertyChanged("Type");
		
		m_TemplateController.Slots.Copy(inventory_slots);
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
		return "DayZEditor/GUI/layouts/Inventory/EditorInventoryEditorWearable.layout";
	}
}


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
	
	ref ObservableCollection<EditorWearableListItem> WearableItems = new ObservableCollection<EditorWearableListItem>(this);
	
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
	
	ref map<string, ref array<ref EditorWearableListItem>> LoadedWearableItems = new map<string, ref array<ref EditorWearableListItem>>();
	
	void EditorInventoryEditorController()
	{
		foreach (string button: RADIO_BUTTONS) {
			// Initialize the arrays!
			string slot = GetRadioCategory(button);
			LoadedWearableItems[slot] = new array<ref EditorWearableListItem>();
		}
		
		LoadWearableItems();
	}
	
	void LoadWearableItems()
	{
		EditorLog.Trace("EditorInventoryEditorHud::LoadPlaceableObjects");
		g_Game.ReportProgress("Loading Wearable Objects");
		
		//array<ref EditorWearableListItem> wearable_items();
		TStringArray config_paths = {};
		config_paths.Insert(CFG_VEHICLESPATH);
		config_paths.Insert(CFG_WEAPONSPATH);
		config_paths.Insert(CFG_MAGAZINESPATH);
		
		foreach (string path: config_paths) {
			for (int i = 0; i < GetWorkbenchGame().ConfigGetChildrenCount(path); i++) {
				string type;
				TStringArray inventory_slots = {};
		        GetWorkbenchGame().ConfigGetChildName(path, i, type);
				Print(path + type);
				GetGame().ConfigGetTextArray(path + type, inventory_slots);
				EditorWearableListItem wearable_item = new EditorWearableListItem(type, inventory_slots);
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
		
		//return placeable_items;
	}
	
	string GetRadioCategory(string radio_button)
	{
		switch (radio_button) {
			case "ShoulderLeft": return "Shoulder";
			case "ShoulderRight": return "LeftHand";
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
	
	override void PropertyChanged(string property_name)
	{
		// Radio Button Logic
		foreach (string button: RADIO_BUTTONS) {
			if (button == property_name) {
				continue;
			}
			
			EnScript.SetClassVar(this, button, 0, false);
			NotifyPropertyChanged(button, false);
		}
	}
}