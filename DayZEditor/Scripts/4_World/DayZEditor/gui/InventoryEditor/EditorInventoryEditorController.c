class EditorInventoryEditorController: ViewController
{
	static const ref TIntArray BLACKLISTED_ATTACHMENTS = {
		InventorySlots.GetSlotIdFromString("LeftHand")
	};
	
	static const ref array<CarFluid> FLUID_TYPES = {
		CarFluid.FUEL,
		CarFluid.BRAKE,
		CarFluid.OIL,
		CarFluid.COOLANT,
	};
	
	protected EntityAI m_Entity;
	string SearchBarLeft;
	string SearchBarLeftIcon = "set:dayz_editor_gui image:search";
	
	string SearchBarRight;
	string SearchBarRightIcon = "set:dayz_editor_gui image:search";
	EntityAI CurrentActiveItem;

	ref map<int, ref array<ref EditorWearableItem>> LoadedWearableItems = new map<int, ref array<ref EditorWearableItem>>();
	
	ref ObservableCollection<ref EditorInventoryAttachmentSlot> AttachmentSlotCategories = new ObservableCollection<ref EditorInventoryAttachmentSlot>(this);	
	ref ObservableCollection<ref EditorWearableListItem> WearableItems = new ObservableCollection<ref EditorWearableListItem>(this);
	ref ObservableCollection<ref EditorInventoryAttachmentSlot> CurrentItemAttachmentSlotCategories = new ObservableCollection<ref EditorInventoryAttachmentSlot>(this);	
	ref ObservableCollection<ref EditorWearableListItem> CurrentItemAttachments = new ObservableCollection<ref EditorWearableListItem>(this);
	ref ObservableCollection<ref EditorInventoryFluidSlot> FluidSliders = new ObservableCollection<ref EditorInventoryFluidSlot>(this);

	ScrollWidget ItemSelectorScrollbar;
	ScrollWidget AttachmentSelectorScrollbar;
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
					int id = InventorySlots.GetSlotIdFromString(inventory_slot);
					if (!LoadedWearableItems[id]) {
						LoadedWearableItems[id] = {};
					}
					
					LoadedWearableItems[id].Insert(wearable_item);
				}
				
				// VERY special case :peepoHappy:
				// because you can literally put ANYTHING in your hands
				int hands_id = InventorySlots.GetSlotIdFromString("Hands");
				if (!LoadedWearableItems[hands_id]) {
					LoadedWearableItems[hands_id] = {};
				}
				
				if (GetGame().IsKindOf(type, "Inventory_Base") || GetGame().IsKindOf(type, "Weapon_Base")) {
					LoadedWearableItems[hands_id].Insert(wearable_item);
				}
		    }
		}
	}
	
	void SetEntity(notnull EntityAI entity)
	{
		m_Entity = entity;
				
		TIntArray attachment_slots = GetAttachmentSlotsFromEntity(m_Entity);
		foreach (int slot: attachment_slots) {
			if (BLACKLISTED_ATTACHMENTS.Find(slot) != -1) {
				continue;
			}
			
			EditorInventoryAttachmentSlot attachment_slot = new EditorInventoryAttachmentSlot(slot);
			attachment_slot.OnItemSelected.Insert(OnAttachmentSlotSelected);
			AttachmentSlotCategories.Insert(attachment_slot);
		}
				
		// Register all fluids for cars
		Car car = Car.Cast(m_Entity);
		foreach (CarFluid fluid: FLUID_TYPES) {
			if (car && car.GetFluidCapacity(fluid) > 0) {
				EditorInventoryFluidSlot fluid_slot = new EditorInventoryFluidSlot(fluid, car.GetFluidCapacity(fluid), car.GetFluidFraction(fluid));
				fluid_slot.OnValueChanged.Insert(OnFluidAmountChanged);
				FluidSliders.Insert(fluid_slot);
			}
		}
						
		// Sets default enable			
		if (AttachmentSlotCategories[0]) {
			AttachmentSlotCategories[0].GetTemplateController().State = true;
			AttachmentSlotCategories[0].GetTemplateController().NotifyPropertyChanged("State");
		}
	}
	
	EditorInventoryData GetInventoryData()
	{
		EditorInventoryData data();
		data.AssignFromEntity(m_Entity);
		return data;
	}
	
	void AssignFromData(EditorInventoryData data)
	{
		data.AssignToEntity(m_Entity);
	}
	
	static TIntArray GetAttachmentSlotsFromEntity(EntityAI entity)
	{
		TIntArray slots = {};
		GameInventory inventory = entity.GetInventory();
		for (int i = 0; i < inventory.GetAttachmentSlotsCount(); i++) {
			slots.Insert(inventory.GetAttachmentSlotId(i));
		}
		
		return slots;
	}
	
	PlayerBase GetEntityAsPlayer()
	{
		return PlayerBase.Cast(m_Entity);
	}
	
	void SetCurrentActiveItem(EntityAI current_item)
	{
		CurrentActiveItem = current_item;
		
		if (!CurrentActiveItem) {
			AttachmentSelectorPanel.Show(false);
			AttachmentFilterSelectPanel.Show(false);
			return;
		}
		
		// Populate right side attachments list
		CurrentItemAttachmentSlotCategories.Clear();
		TIntArray new_attachments = GetAttachmentSlotsFromEntity(CurrentActiveItem);
		foreach (int slot: new_attachments) {
			if (BLACKLISTED_ATTACHMENTS.Find(slot) != -1) {
				continue;
			}
			
			EditorInventoryAttachmentSlot attachment_slot = new EditorInventoryAttachmentSlot(slot);
			attachment_slot.OnItemSelected.Insert(OnCurrentItemAttachmentSlotSelected);
			CurrentItemAttachmentSlotCategories.Insert(attachment_slot);
		}		
		
		// Sets default category to ON
		if (CurrentItemAttachmentSlotCategories[0]) {
			CurrentItemAttachmentSlotCategories[0].GetTemplateController().State = true;
			CurrentItemAttachmentSlotCategories[0].GetTemplateController().NotifyPropertyChanged("State");
		}
		
		AttachmentSelectorPanel.Show(CurrentItemAttachmentSlotCategories.Count() > 0);
		AttachmentFilterSelectPanel.Show(CurrentItemAttachmentSlotCategories.Count() > 0);
	}
	
	void OnListItemSelected(EditorWearableListItem list_item, EditorWearableItem wearable_item)
	{		
		// Very special, probably use some type of enum in the future
		if (list_item.GetSlot() == InventorySlots.GetSlotIdFromString("Hands") && GetEntityAsPlayer()) {
			GetGame().ObjectDelete(GetEntityAsPlayer().GetHumanInventory().GetEntityInHands());
			if (list_item == EmptyItem) return;
			
			SetCurrentActiveItem(GetEntityAsPlayer().GetHumanInventory().CreateInHands(wearable_item.Type));
		} 
		
		else {
			int slot_id = list_item.GetSlot();
			
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
			if (AttachmentSlotCategories[i] != attachment_slot) { // not adding bc drunk && AttachmentSlotCategories[i].GetTemplateController().State
				AttachmentSlotCategories[i].GetTemplateController().State = false;
				AttachmentSlotCategories[i].GetTemplateController().NotifyPropertyChanged("State", false);
				continue;
			}
			
			WearableItems.Clear();
			
			int inventory_slot = AttachmentSlotCategories[i].GetSlot();			
			string slot_name = InventorySlots.GetSlotName(inventory_slot);	
			
			// Register empty item
			EditorWearableListItem empty_list_item = new EditorWearableListItem(EmptyItem, inventory_slot);			
		
			// Slot is empty
			if (slot_name != "Hands" && !m_Entity.GetInventory().FindAttachment(inventory_slot)) {
				empty_list_item.Select();
			}
			
			// Handle Hands
			if (slot_name == "Hands" && player && !player.GetHumanInventory().GetEntityInHands()) {
				empty_list_item.Select();
			}
			
			empty_list_item.OnItemSelected.Insert(OnListItemSelected);
			WearableItems.Insert(empty_list_item);	
			
			if (!LoadedWearableItems[inventory_slot]) {
				continue;
			}
			
			foreach (EditorWearableItem wearable: LoadedWearableItems[inventory_slot]) {
				// Great fix for weird items like magazines, which all rely on the same attachment slot name
				// this function does not work on just players yet, only items, need to fix
				//if (!CanAddAsAttachment(CurrentActiveItem, wearable.Type)) {
				//	continue;
				//}
				
				// This is the part where we need to call NEW, not before
				EditorWearableListItem list_item = new EditorWearableListItem(wearable, inventory_slot);
				list_item.OnItemSelected.Insert(OnListItemSelected);
				
				// update by search bar
				list_item.GetLayoutRoot().Show(list_item.FilterType(SearchBarLeft)); 				
				WearableItems.Insert(list_item);
				
				// Assign active item from slot
				EntityAI slot_item = m_Entity.GetInventory().FindAttachment(inventory_slot);
				if (!slot_item) {
					continue;
				}
				
				if (slot_item.GetType() == wearable.Type) {
					list_item.Select();
				}
				
				// Handle Hands
				if (slot_name == "Hands" && player && player.GetHumanInventory().GetEntityInHands()) {
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
		int slot_id = list_item.GetSlot();
		
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
			
			int inventory_slot = CurrentItemAttachmentSlotCategories[i].GetSlot();			
			string slot_name = InventorySlots.GetSlotName(inventory_slot);
			
			// Register empty item
			EditorWearableListItem empty_list_item = new EditorWearableListItem(EmptyItem, inventory_slot);			
		
			// Slot is empty
			if (slot_name != "Hands" && !m_Entity.GetInventory().FindAttachment(inventory_slot)) {
				empty_list_item.Select();
			}
			
			// Handle Hands
			if (slot_name == "Hands" && player && !player.GetHumanInventory().GetEntityInHands()) {
				empty_list_item.Select();
			}
			
			empty_list_item.OnItemSelected.Insert(OnCurrentItemAttachmentSelected);
			CurrentItemAttachments.Insert(empty_list_item);	
			
			foreach (EditorWearableItem wearable: LoadedWearableItems[inventory_slot]) {
				// Great fix for weird items like magazines, which all rely on the same attachment slot name
				if (!CanAddAsAttachment(CurrentActiveItem, wearable.Type)) {
					continue;
				}
				
				// This is the part where we need to call NEW, not before
				EditorWearableListItem list_item = new EditorWearableListItem(wearable, inventory_slot);
				list_item.OnItemSelected.Insert(OnCurrentItemAttachmentSelected);
				
				// update by search bar
				list_item.GetLayoutRoot().Show(list_item.FilterType(SearchBarRight)); 				
				CurrentItemAttachments.Insert(list_item);
				
				// Assign active item from slot
				EntityAI slot_item = m_Entity.GetInventory().FindAttachment(inventory_slot);
				if (!slot_item) {
					continue;
				}
				
				if (slot_item.GetType() == wearable.Type) {
					list_item.Select();
				}
				
				// Handle Hands
				if (slot_name == "Hands" && player && player.GetHumanInventory().GetEntityInHands()) {
					if (player.GetHumanInventory().GetEntityInHands().GetType() == wearable.Type) {
						list_item.Select();
					}
				}
			}
			
			AttachmentSelectorScrollbar.VScrollToPos(0);
		}
	}
	
	void OnFluidAmountChanged(CarFluid fluid_type, float amount)
	{
		Car car = Car.Cast(m_Entity);
		if (!car) {
			return;
		}
		
		car.LeakAll(fluid_type);
		car.Fill(fluid_type, amount);
	}
	
	static bool CanAddAsAttachment(EntityAI item, string attachment)
	{
		if (!item) {
			return false;
		}
		
		EntityAI attachment_item = item.GetInventory().CreateAttachment(attachment);
		if (!attachment_item) {
			return false;
		}
		
		GetGame().ObjectDelete(attachment_item);
		return true;
	}
	
	override void PropertyChanged(string property_name)
	{		
		switch (property_name) {
			
			case "SearchBarRight": {
				for (int i = 0; i < CurrentItemAttachments.Count(); i++) {
					CurrentItemAttachments[i].GetLayoutRoot().Show(CurrentItemAttachments[i].FilterType(SearchBarRight)); 
				}
				
				ItemSelectorScrollbar.VScrollToPos(0);
				if (SearchBarRight.Length() > 0) {
					SearchBarRightIcon = "set:dayz_gui image:icon_x";
				} else {
					SearchBarRightIcon = "set:dayz_editor_gui image:search";
				}
				
				NotifyPropertyChanged("SearchBarRightIcon");
				break;
			}
			
			case "SearchBarLeft": {
				for (int j = 0; j < WearableItems.Count(); j++) {
					WearableItems[j].GetLayoutRoot().Show(WearableItems[j].FilterType(SearchBarLeft)); 
				}
				
				AttachmentSelectorScrollbar.VScrollToPos(0);
				if (SearchBarLeft.Length() > 0) {
					SearchBarLeftIcon = "set:dayz_gui image:icon_x";
				} else {
					SearchBarLeftIcon = "set:dayz_editor_gui image:search";
				}
				
				NotifyPropertyChanged("SearchBarLeftIcon");
				break;
			}			
		}
	}
}