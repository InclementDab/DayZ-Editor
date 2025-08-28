 class EditorInventoryEditorHud: ScriptViewMenu
{
	static const string FILE_EXTENSION = ".dzeinv";
	
	protected EditorInventoryEditorController m_TemplateController;
	
	protected EditorInventoryEditorCamera m_Camera;
	protected EntityAI m_Entity;
	protected EditorObject m_EditorObject;

	Widget OpenInventoryOutline;
	EditBoxWidget ItemSelectorSearchBar, AttachmentSelectorSearchBar;
	
	protected ref EditorFileDialog m_FileDialog;
	
	void EditorInventoryEditorHud(notnull EntityAI entity, EditorObjectData storage_data = null)
	{
		m_Entity = entity;
		m_EditorObject = GetEditor().GetEditorObject(m_Entity);
		m_Camera = EditorInventoryEditorCamera.Cast(GetGame().CreateObject("EditorInventoryEditorCamera", m_Entity.GetPosition()));
		m_Camera.SetTarget(m_Entity);
		
		vector size = ObjectGetSize(m_Entity);		
		vector pos = m_Entity.GetPosition();
		vector dir = m_Entity.GetDirection();
		vector ground = pos + dir * 3;
		ground[1] = GetGame().SurfaceY(ground[0], ground[2]);
		vector camera_pos = ground + Vector(0, 1.5, 0);
			
		m_Camera.SetPosition(camera_pos);
		m_Camera.Update();
		m_Camera.SetActive(true);
		
		m_TemplateController = EditorInventoryEditorController.Cast(m_Controller);
		m_TemplateController.SetEntity(m_Entity, storage_data);
		
		m_Entity.DisableSimulation(false);
		GetGame().GetUIManager().ShowCursor(true);
		SetFocus(null);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(PPEffects.ResetAll);
	}
	
	void ~EditorInventoryEditorHud()
	{
		GetGame().ObjectDelete(m_Camera);
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		Input input = GetGame().GetInput();
		if (input.LocalPress("EditorToggleUI")) {
			Show(!IsVisible());
		}
		
		if (input.LocalPress("EditorToggleCursor")) {
			GetGame().GetUIManager().ShowCursor(!GetGame().GetUIManager().IsCursorVisible());
		}

		PlayerBase player_entity = PlayerBase.Cast(m_Entity);
		OpenInventoryOutline.Show(player_entity != null);
	}
	
	override typename GetControllerType()
	{
		return EditorInventoryEditorController;
	}
	
	void ClearExecute(ButtonCommandArgs args)
	{
		// Set current active to Empty
		for (int i = 0; i < m_TemplateController.WearableItems.Count(); i++) {
			if (m_TemplateController.WearableItems[i].GetWearableItem() == EditorInventoryEditorController.EmptyItem) {
				m_TemplateController.WearableItems[i].ListItemExecute(null);
			}
		}
		
		// Just delete the rest of the stuff
		m_Entity.ClearInventory();
	}
	
	void OpenInventoryExecute(ButtonCommandArgs args)
	{		
		PlayerBase player_entity = PlayerBase.Cast(m_Entity);
		if (player_entity) {
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(GetEditor().ControlPlayer, 0, false, player_entity);
		}
	}
	
	void ExitExecute(ButtonCommandArgs args)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(GetEditor().StopInventoryEditor, 0, false);
	}
	
	void ImportExecute(ButtonCommandArgs args)
	{
		m_FileDialog = new EditorFileDialog(EditorSpawnPresetFile, ScriptCaller.Create(OnImportSelected), eDialogMode.IMPORT, eDialogFlags.ALLOW_DOUBLE_CLICK, string.Empty);
	}
	
	void ExportExecute(ButtonCommandArgs args)
	{
		m_FileDialog = new EditorFileDialog(EditorSpawnPresetFile, ScriptCaller.Create(OnExportSelected), eDialogMode.EXPORT, eDialogFlags.WARN_ON_OVERWRITE, string.Empty);
	}

	protected void OnImportSelected(string file)
	{
		if (!FileExist(file)) {
			return;
		}
		
		m_Entity.ClearInventory();

		string error;
		PlayerSpawnPreset preset;
		if (!JsonFileLoader<PlayerSpawnPreset>.LoadFile(file, preset, error)) {
			EditorLog.Warning(error);
			return;
		}

		PlayerBase player_entity = PlayerBase.Cast(m_Entity);
		if (player_entity) {
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PlayerSpawnHandler.ProcessEquipmentData, 0, false, player_entity, preset);
		}
	}

	protected void OnExportSelected(string file)
	{
		string error;
		PlayerSpawnPreset preset = new PlayerSpawnPreset();
		preset.attachmentSlotItemSets = {}; 
		preset.discreteUnsortedItemSets = {}; // nothing makes sense BI why do u do this crap
		preset.name = m_EditorObject.GetDisplayName();
		preset.characterTypes = { m_Entity.GetType() };
		preset.spawnWeight = 1;
		
		array<EntityAI> entities = {};
		m_Entity.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
		foreach (EntityAI entity: entities) {
			if (m_Entity == entity) {
				continue;
			}

			InventoryLocation il = new InventoryLocation();
			entity.GetInventory().GetCurrentInventoryLocation(il);
			int slot_id = il.GetSlot();
			string slot_name = InventorySlots.GetSlotName(slot_id);

			auto slot_item_data = new PlayerSpawnPresetDiscreteItemSetSlotData();
			slot_item_data.spawnWeight = 1;
			slot_item_data.simpleChildrenUseDefaultAttributes = true;
			slot_item_data.attributes = new PlayerSpawnAttributesData();
			slot_item_data.attributes.healthMin = 1;
			slot_item_data.attributes.healthMax = 1;
			slot_item_data.itemType = entity.GetType();
			slot_item_data.simpleChildrenTypes = {};

			array<EntityAI> children_entities = {};
			entity.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, children_entities);
			foreach (EntityAI child_entity: children_entities) {
				if (child_entity == entity) {
					continue;
				}

				slot_item_data.simpleChildrenTypes.Insert(child_entity.GetType());
			}

			PlayerSpawnPresetSlotData slot_data = new PlayerSpawnPresetSlotData();
			slot_data.slotName = slot_name;
			slot_data.discreteItemSets = {
				slot_item_data
			};
			
			preset.attachmentSlotItemSets.Insert(slot_data);
		}
		
		DeleteFile(file);
		if (!JsonFileLoader<PlayerSpawnPreset>.SaveFile(file, preset, error)) {
			EditorLog.Warning(error);
			return;
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {			
			case AttachmentSelectorSearchBar:
			case ItemSelectorSearchBar: {
				return true;
			}
		}

		return super.OnMouseEnter(w, x, y);
	}
	
	private void ImportExecuteThread()
	{
		EditorFileDialog_DEPRECATED file_dialog("Import Inventory Data", "*.dzeinv", "", "Import");
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		file_name = SystemPath.Combine(Editor.ROOT_DIRECTORY, file_name);
		EditorFileManager.GetSafeFileName(file_name, FILE_EXTENSION);
		FileSerializer serializer();
		if (!serializer.Open(file_name, FileMode.READ)) {
			return;
		}
		
		EditorInventoryData data();
		if (!data.Read(serializer, 0)) {
			serializer.Close();
			return;
		}
		
		serializer.Close();
		m_TemplateController.AssignFromData(data);
	}
	
	private void ExportExecuteThread()
	{
		EditorInventoryData data = m_TemplateController.GetInventoryData();
		EditorFileDialog_DEPRECATED file_dialog("Export Inventory Data", "*.dzeinv", "", "Export");
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		file_name = SystemPath.Combine(Editor.ROOT_DIRECTORY, file_name);
		EditorFileManager.GetSafeFileName(file_name, FILE_EXTENSION);
		FileSerializer serializer();
		if (!serializer.Open(file_name, FileMode.WRITE)) {
			return;
		}
		
		data.Write(serializer, 0); // version means nothing here
		serializer.Close();
	}
	
	override bool UseMouse()
	{
		return true;
	}
	
	EntityAI GetEntity()
	{
		return m_Entity;
	}
		
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/Inventory/Inventory.layout";
	}
}