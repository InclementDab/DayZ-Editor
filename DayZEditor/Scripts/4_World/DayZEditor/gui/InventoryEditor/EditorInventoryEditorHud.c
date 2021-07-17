class EditorInventoryEditorHud: ScriptViewTemplate<EditorInventoryEditorController>
{
	protected EditorInventoryEditorCamera m_Camera;
	protected PlayerBase m_Player;
	
	void EditorInventoryEditorHud(PlayerBase player)
	{
		m_Player = player;
		m_Camera = EditorInventoryEditorCamera.Cast(GetGame().CreateObject("EditorInventoryEditorCamera", m_Player.GetPosition()));
		
		vector pos = m_Player.GetPosition();
		vector dir = m_Player.GetDirection();
		
		vector target_pos = pos + "0 1.2 0" + (dir * 3.0) + (dir.Perpend() * 1.0);
		//m_Camera.LerpToPosition(target_pos, 1.0);
		m_Camera.SetPosition(target_pos);
		m_Camera.Update();
		m_Camera.SetActive(true);
		
		m_TemplateController.Player = m_Player;
		
		// Just default
		m_TemplateController.ShoulderLeft = true;
		m_TemplateController.NotifyPropertyChanged("ShoulderLeft");
	}
	
	void ~EditorInventoryEditorHud()
	{
		m_Camera.SetActive(false);
		GetGame().ObjectDelete(m_Camera);
	}
	
	PlayerBase GetPlayer()
	{
		return m_Player;
	}
		
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/Inventory/Inventory.layout";
	}
}