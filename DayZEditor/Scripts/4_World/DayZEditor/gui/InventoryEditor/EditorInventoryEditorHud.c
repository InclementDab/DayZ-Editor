class EditorInventoryEditorHud: ScriptViewTemplate<EditorInventoryEditorController>
{
	protected EditorInventoryEditorCamera m_Camera;
	protected EntityAI m_Entity;
	
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
		m_Camera.SetActive(true);
		
		m_TemplateController.SetEntity(m_Entity);
	}
	
	void ~EditorInventoryEditorHud()
	{
		m_Camera.SetActive(false);
		GetGame().ObjectDelete(m_Camera);
	}
	
	void ImportExecute(ButtonCommandArgs args)
	{
		
	}
	
	void ExportExecute(ButtonCommandArgs args)
	{
		EditorInventoryData data = m_TemplateController.GetInventoryData();
		Print(data);
		Print(data.Type);
		foreach (EditorInventoryAttachmentData att: data.Attachments) {
			Print(att);
		}
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