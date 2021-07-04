class EditorInventoryEditorHud: ScriptViewTemplate<EditorInventoryEditorController>
{
	protected EditorInventoryEditorCamera m_Camera;
	
	void EditorInventoryEditorHud()
	{
		m_Camera = EditorInventoryEditorCamera.Cast(GetGame().CreateObject("EditorInventoryEditorCamera", GetGame().GetPlayer().GetPosition()));
		
		vector pos = GetGame().GetPlayer().GetPosition();
		vector dir = GetGame().GetPlayer().GetDirection();
		
		vector target_pos = pos + "0 1.2 0" + (dir * 3.0) + (dir.Perpend() * 1.0);
		//m_Camera.LerpToPosition(target_pos, 1.0);
		m_Camera.SetPosition(target_pos);
		m_Camera.Update();
		m_Camera.SetActive(true);
		
		// Just default
		m_TemplateController.ShoulderLeft = true;
		m_TemplateController.NotifyPropertyChanged("ShoulderLeft");
	}
	
	void ~EditorInventoryEditorHud()
	{
		m_Camera.SetActive(false);
		GetGame().ObjectDelete(m_Camera);
	}
		
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/Inventory/Inventory.layout";
	}
}