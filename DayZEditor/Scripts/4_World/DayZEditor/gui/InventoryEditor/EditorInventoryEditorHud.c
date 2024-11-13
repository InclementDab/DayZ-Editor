class EditorInventoryEditorHud: ScriptViewMenu
{
	static const string FILE_EXTENSION = ".dzeinv";
	
	protected EditorInventoryEditorController m_TemplateController;
	
	protected EditorInventoryEditorCamera m_Camera;
	protected EntityAI m_Entity;

	Widget OpenInventoryOutline;
	
	void EditorInventoryEditorHud(notnull EntityAI entity)
	{
		m_Entity = entity;
		m_Camera = EditorInventoryEditorCamera.Cast(GetGame().CreateObject("EditorInventoryEditorCamera", m_Entity.GetPosition()));
		m_Camera.SetTarget(m_Entity);
		
		vector size = ObjectGetSize(m_Entity);		
		vector pos = m_Entity.GetPosition();
		vector dir = m_Entity.GetDirection();
		
		vector target_pos = pos + Vector(0, size[1] / 2, 0) + (dir * size[2]) + (dir.Perpend() * (size[0] / 2));
		//m_Camera.LerpToPosition(target_pos, 1.0);
		m_Camera.SetPosition(target_pos);
		m_Camera.Update();
		//GetGame().SelectPlayer(null, null);

		m_Camera.SetActive(true);
		
		m_TemplateController = EditorInventoryEditorController.Cast(m_Controller);
		m_TemplateController.SetEntity(m_Entity);
		
		GetGame().GetUIManager().ShowCursor(true);
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
		array<EntityAI> items = {};
		m_Entity.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items) {
			if (item.IsInherited(PlayerBase)) { // yep
				continue;
			}
			
			GetGame().ObjectDelete(item);
		}
	}
	
	void OpenInventoryExecute(ButtonCommandArgs args)
	{		
		PlayerBase player_entity = PlayerBase.Cast(m_Entity);
		if (player_entity) {
			GetEditor().SetPlayer(player_entity);
			GetEditor().SetActive(false);
		}
	}
	
	void ExitExecute(ButtonCommandArgs args)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(GetEditor().SetActive, 0, false, true);
	}
	
	void ImportExecute(ButtonCommandArgs args)
	{
		thread ImportExecuteThread();
	}
	
	void ExportExecute(ButtonCommandArgs args)
	{
		thread ExportExecuteThread();
	}
	
	private void ImportExecuteThread()
	{
		EditorFileDialog file_dialog("Import Inventory Data", "*.dzeinv", "", "Import");
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		file_name = Editor.ROOT_DIRECTORY + file_name;
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
		EditorFileDialog file_dialog("Export Inventory Data", "*.dzeinv", "", "Export");
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		file_name = Editor.ROOT_DIRECTORY + file_name;
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
		return false;
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