class EditorNametag: ScriptView
{
	TextWidget Text;
	protected Object m_Attachment;
	
	void EditorNametag(string text, Object attachment)
	{
		Text.SetText(text);
		m_Attachment = attachment;
	}
	
	override void Update(float dt)
	{
		vector screen_pos = GetGame().GetScreenPos(m_Attachment.GetPosition());
		if (screen_pos[2] > 100) {
			m_LayoutRoot.SetAlpha(0);
			return;
		}
		
		float distance_value = Math.InverseLerp(100, 1, Math.Clamp(screen_pos[2], 0, 100));		
		m_LayoutRoot.SetPos(screen_pos[0], screen_pos[1]);
		m_LayoutRoot.SetAlpha(distance_value);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorNametag.layout";
	}
}