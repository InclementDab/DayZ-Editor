class EditorInventoryEditorController: ViewController
{
	static const ref TStringArray BLACKLISTED_ATTACHMENTS = {
		"LeftHand"
	};
	
	protected EntityAI m_Entity;
	string SearchBarData;
	string SearchBarIcon = "set:dayz_editor_gui image:search";
	ScrollWidget ItemSelectorScrollbar;
	EntityAI CurrentActiveItem;

	ref ObservableCollection<ref EditorInventoryAttachmentSlot> AttachmentSlotCategories = new ObservableCollection<ref EditorInventoryAttachmentSlot>(this);	
	ref ObservableCollection<ref EditorWearableListItem> WearableItems = new ObservableCollection<ref EditorWearableListItem>(this);
	ref map<string, ref array<ref EditorWearableItem>> LoadedWearableItems = new map<string, ref array<ref EditorWearableItem>>();
	ref TStringArray LoadedAttachmentSlots = {};
	
	ref ObservableCollection<ref EditorInventoryAttachmentSlot> CurrentItemAttachmentSlotCategories = new ObservableCollection<ref EditorInventoryAttachmentSlot>(this);	
	ref ObservableCollection<ref EditorWearableListItem> CurrentItemAttachments = new ObservableCollection<ref EditorWearableListItem>(this);

	Widget AttachmentSelectorPanel;
	Widget AttachmentFilterSelectPanel;
	
	const ref EditorWearableItem EmptyItem = new EditorWearableItem("<empty>", "<empty>", {"any"});
	
	void EditorInventoryEditorController()
	{
		EditorLog.Trace("EditorInventoryEditorController");
		g_Game.ReportProgress("Loading Wearable Objects");
		// Load all of the possible wearables in the game		
		TStringArray config_paths = {};
		config_paths.Insert(CFG_VEHICLESPATH);
		config_paths.Insert(CFG_WEAPONSPATH);
		config_paths.Insert(CFG_MAGAZINESPATH);
		
		foreach (string path: config_paths) {
			for (int i = 0; i < GetGame().ConfigGetChildrenCount(path); i++) {
				string type;
				TStringArray inventory_slots = {};
		        GetGame().ConfigGetChildName(path, i, type);
				string object_full_path = path + " " + type;
				GetGame().ConfigGetTextArray(object_full_path + " inventorySlot", inventory_slots);
				if (GetGame().ConfigGetInt(object_full_path + " scope") < 2) { // maybe 2 is the way
					continue;
				}
				
				EditorWearableItem wearable_item = new EditorWearableItem(type, GetGame().ConfigGetTextOut(object_full_path + " displayName"), inventory_slots);
				foreach (string inventory_slot: inventory_slots) {
					// Check if its a supported inventory slot
					if (!LoadedWearableItems[inventory_slot]) {
						LoadedWearableItems[inventory_slot] = {};
					}
					
					LoadedWearableItems[inventory_slot].Insert(wearable_item);
				}
				
				// VERY special case :peepoHappy:
				// because you can literally put ANYTHING in your hands
				if (!LoadedWearableItems["Hands"]) {
					LoadedWearableItems["Hands"] = {};
				}
				
				if (GetGame().IsKindOf(type, "Inventory_Base") || GetGame().IsKindOf(type, "Weapon_Base")) {
					LoadedWearableItems["Hands"].Insert(wearable_item);
				}
		    }
		}
	}
	
	void SetEntity(notnull EntityAI entity)
	{
		m_Entity = entity;
				
		LoadedAttachmentSlots = GetAttachmentSlotsFromEntity(m_Entity);
		foreach (string slot: LoadedAttachmentSlots) {
			if (BLACKLISTED_ATTACHMENTS.Find(slot) != -1) {
				continue;
			}
			
			EditorInventoryAttachmentSlot attachment_slot = new EditorInventoryAttachmentSlot(slot, GetSlotImageFromSlotName(slot));
			attachment_slot.OnItemSelected.Insert(OnAttachmentSlotSelected);
			AttachmentSlotCategories.Insert(attachment_slot);
		}
				
		// Sets default enable			
		if (AttachmentSlotCategories[0]) {
			AttachmentSlotCategories[0].GetTemplateController().State = true;
			AttachmentSlotCategories[0].GetTemplateController().NotifyPropertyChanged("State");
		}
	}
	
	static TStringArray GetAttachmentSlotsFromEntity(EntityAI entity)
	{
		map<int, string> slots = new map<int, string>();
		GameInventory inventory = entity.GetInventory();
		for (int i = 0; i < inventory.GetAttachmentSlotsCount(); i++) {
			int id = inventory.GetAttachmentSlotId(i);
			slots[id] = InventorySlots.GetSlotName(id);
		}
		
		return slots.GetValueArray();
	}
	
	static string GetSlotImageFromSlotName(string slot_name)
	{
		// crackhead shit
		return GetGame().ConfigGetTextOut(string.Format("CfgSlots Slot_%1 ghostIcon", slot_name));
	}
	
	PlayerBase GetEntityAsPlayer()
	{
		return PlayerBase.Cast(m_Entity);
	}
	
	void SetCurrentActiveItem(EntityAI current_item)
	{
		CurrentActiveItem = current_item;
		
		// Populate right side attachments list
		TStringArray new_attachments = GetAttachmentSlotsFromEntity(CurrentActiveItem);
		
		//CurrentItemAttachments.Clear();
		CurrentItemAttachmentSlotCategories.Clear();
		foreach (string slot: new_attachments) {
			if (BLACKLISTED_ATTACHMENTS.Find(slot) != -1) {
				continue;
			}
			
			EditorInventoryAttachmentSlot attachment_slot = new EditorInventoryAttachmentSlot(slot, GetSlotImageFromSlotName(slot));
			attachment_slot.OnItemSelected.Insert(OnCurrentItemAttachmentSlotSelected);
			CurrentItemAttachmentSlotCategories.Insert(attachment_slot);
		}
				
		/*
		// Sets default category to ON
		if (CurrentItemAttachmentSlotCategories[0]) {
			CurrentItemAttachmentSlotCategories[0].GetTemplateController().State = true;
			CurrentItemAttachmentSlotCategories[0].GetTemplateController().NotifyPropertyChanged("State", false);
		}
		
		AttachmentSelectorPanel.Show(CurrentItemAttachmentSlotCategories.Count() > 0);
		AttachmentFilterSelectPanel.Show(CurrentItemAttachmentSlotCategories.Count() > 0);*/
	}
	
	// playerSlots[] = {"Slot_Shoulder","Slot_Melee","Slot_Vest","Slot_Body","Slot_Hips","Slot_Legs","Slot_Back","Slot_Headgear","Slot_Mask","Slot_Eyewear","Slot_Gloves","Slot_Feet","Slot_Armband"};
		
	void OnListItemSelected(EditorWearableListItem list_item, EditorWearableItem wearable_item)
	{		
		// Very special, probably use some type of enum in the future
		if (list_item.GetSlot() == "Hands" && GetEntityAsPlayer()) {
			GetGame().ObjectDelete(GetEntityAsPlayer().GetHumanInventory().GetEntityInHands());
			if (list_item == EmptyItem) return;
			
			SetCurrentActiveItem(GetEntityAsPlayer().GetHumanInventory().CreateInHands(wearable_item.Type));
		} 
		
		else {
			int slot_id = InventorySlots.GetSlotIdFromString(list_item.GetSlot());
			
			// Clear existing item
			GetGame().ObjectDelete(m_Entity.GetInventory().FindAttachment(slot_id));
			if (list_item == EmptyItem) return;
			
			// Create new item on player
			SetCurrentActiveItem(m_Entity.GetInventory().CreateAttachmentEx(wearable_item.Type, slot_id));
		}
		
		// Deselect all other things
		for (int i = 0; i < WearableItems.Count(); i++) {
			WearableItems[i].SetSelected(WearableItems[i] == list_item);
		}
	}
		
	void OnAttachmentSlotSelected(EditorInventoryAttachmentSlot attachment_slot)
	{		
		PlayerBase player = GetEntityAsPlayer();
		
		// Radio Button Logic
		for (int i = 0; i < AttachmentSlotCategories.Count(); i++) {
			
			// Deselect all other attachments
			if (AttachmentSlotCategories[i] != attachment_slot) {
				AttachmentSlotCategories[i].GetTemplateController().State = false;
				AttachmentSlotCategories[i].GetTemplateController().NotifyPropertyChanged("State", false);
				continue;
			}
			
			WearableItems.Clear();
			
			string inventory_slot = AttachmentSlotCategories[i].GetSlot();			
						
			// Register empty item
			EditorWearableListItem empty_list_item = new EditorWearableListItem(EmptyItem, inventory_slot);			
		
			// Slot is empty
			if (inventory_slot != "Hands" && !m_Entity.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString(inventory_slot))) {
				empty_list_item.Select();
			}
			
			// Handle Hands
			if (inventory_slot == "Hands" && player && !player.GetHumanInventory().GetEntityInHands()) {
				empty_list_item.Select();
			}
			
			empty_list_item.OnItemSelected.Insert(OnListItemSelected);
			WearableItems.Insert(empty_list_item);	
			
			if (!LoadedWearableItems[inventory_slot]) {
				continue;
			}
			
			foreach (EditorWearableItem wearable: LoadedWearableItems[inventory_slot]) {
				// This is the part where we need to call NEW, not before
				EditorWearableListItem list_item = new EditorWearableListItem(wearable, inventory_slot);
				list_item.OnItemSelected.Insert(OnListItemSelected);
				
				// update by search bar
				list_item.GetLayoutRoot().Show(list_item.FilterType(SearchBarData)); 
				WearableItems.Insert(list_item);
				
				// Assign active item from slot
				EntityAI slot_item = m_Entity.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString(inventory_slot));
				if (slot_item && slot_item.GetType() == wearable.Type) {
					//SetCurrentActiveItem(slot_item);
					list_item.Select();
				}
				
				// Handle Hands
				if (inventory_slot == "Hands" && player && player.GetHumanInventory().GetEntityInHands()) {
					if (player.GetHumanInventory().GetEntityInHands().GetType() == wearable.Type) {
						list_item.Select();
					}
				}
			}
			
			// Reset scroll bar
			ItemSelectorScrollbar.VScrollToPos(0);			
		}
	}
	
	// This is the stuff on the right side
	void OnCurrentItemAttachmentSelected(EditorWearableListItem list_item, EditorWearableItem wearable_item)
	{		
		int slot_id = InventorySlots.GetSlotIdFromString(list_item.GetSlot());
		
		// Clear existing item
		GetGame().ObjectDelete(CurrentActiveItem.GetInventory().FindAttachment(slot_id));
		if (list_item == EmptyItem) return;
		
		// Create new item on player
		CurrentActiveItem.GetInventory().CreateAttachmentEx(wearable_item.Type, slot_id);
	
		
		// Deselect all other things
		for (int i = 0; i < CurrentItemAttachments.Count(); i++) {
			CurrentItemAttachments[i].SetSelected(CurrentItemAttachments[i] == list_item);
		}
	}
	
	void OnCurrentItemAttachmentSlotSelected(EditorInventoryAttachmentSlot attachment_slot)
	{		
		PlayerBase player = GetEntityAsPlayer();
		
		// Radio Button Logic
		for (int i = 0; i < CurrentItemAttachmentSlotCategories.Count(); i++) {
			
			// Deselect all other attachments
			if (CurrentItemAttachmentSlotCategories[i] != attachment_slot) {
				CurrentItemAttachmentSlotCategories[i].GetTemplateController().State = false;
				CurrentItemAttachmentSlotCategories[i].GetTemplateController().NotifyPropertyChanged("State", false);
				continue;
			}
			
			CurrentItemAttachments.Clear();
			
			string inventory_slot = CurrentItemAttachmentSlotCategories[i].GetSlot();			

			// Register empty item
			EditorWearableListItem empty_list_item = new EditorWearableListItem(EmptyItem, inventory_slot);			
		
			// Slot is empty
			if (inventory_slot != "Hands" && !m_Entity.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString(inventory_slot))) {
				empty_list_item.Select();
			}
			
			// Handle Hands
			if (inventory_slot == "Hands" && player && !player.GetHumanInventory().GetEntityInHands()) {
				empty_list_item.Select();
			}
			
			empty_list_item.OnItemSelected.Insert(OnCurrentItemAttachmentSelected);
			CurrentItemAttachments.Insert(empty_list_item);	
			
			foreach (EditorWearableItem wearable: LoadedWearableItems[inventory_slot]) {
				// This is the part where we need to call NEW, not before
				EditorWearableListItem list_item = new EditorWearableListItem(wearable, inventory_slot);
				list_item.OnItemSelected.Insert(OnCurrentItemAttachmentSelected);
				
				// update by search bar
				//list_item.GetLayoutRoot().Show(list_item.FilterType(SearchBarData)); 
				CurrentItemAttachments.Insert(list_item);
				
				// Assign active item from slot
				EntityAI slot_item = m_Entity.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString(inventory_slot));
				if (slot_item && slot_item.GetType() == wearable.Type) {
					list_item.Select();
				}
				
				// Handle Hands
				if (inventory_slot == "Hands" && player && player.GetHumanInventory().GetEntityInHands()) {
					if (player.GetHumanInventory().GetEntityInHands().GetType() == wearable.Type) {
						list_item.Select();
					}
				}
			}
		}
	}
	
	override void PropertyChanged(string property_name)
	{		
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