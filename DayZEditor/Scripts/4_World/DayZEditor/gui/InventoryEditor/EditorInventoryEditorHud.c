class EditorInventoryEditorHud: ScriptViewTemplate<EditorInventoryEditorController>
{
	protected EditorInventoryEditorCamera m_Camera;
	protected EntityAI m_Entity;
	
	void EditorInventoryEditorHud(notnull EntityAI entity)
	{
		m_Entity = entity;
		m_Camera = EditorInventoryEditorCamera.Cast(GetGame().CreateObject("EditorInventoryEditorCamera", m_Entity.GetPosition()));
		m_Camera.SetTarget(m_Entity);
		
		vector pos = m_Entity.GetPosition();
		vector dir = m_Entity.GetDirection();
		
		vector target_pos = pos + "0 1.2 0" + (dir * 2.0) + (dir.Perpend() * 0.5);
		//m_Camera.LerpToPosition(target_pos, 1.0);
		m_Camera.SetPosition(target_pos);
		m_Camera.Update();
		m_Camera.SetActive(true);
		
		m_TemplateController.SetEntity(m_Entity);
		
		
		// Just default
		m_TemplateController.ShoulderLeft = true;
		m_TemplateController.NotifyPropertyChanged("ShoulderLeft");
	}
	
	void ~EditorInventoryEditorHud()
	{
		m_Camera.SetActive(false);
		GetGame().ObjectDelete(m_Camera);
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