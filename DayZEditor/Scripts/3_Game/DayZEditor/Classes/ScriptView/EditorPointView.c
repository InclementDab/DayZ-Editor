class EditorPointView: ScriptView
{	
	vector Position;
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		vector screen_position = GetGame().GetScreenPos(Position);
		if (m_LayoutRoot) {
			m_LayoutRoot.SetPos(screen_position[0], screen_position[1]);
			m_LayoutRoot.Show(screen_position[2] > 0);
		}
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorPoint.layout";
	}
}