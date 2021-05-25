/*class EditorAnimationCommand: HumanCommandScript
{
	protected DayZPlayer m_Player;
	protected EditorAnimationManagerTable m_Table;
	protected HumanInputController m_Input;
	
	protected bool m_NeedFinish;
	
	void EditorAnimationCommand(DayZPlayer player, EditorAnimationManagerTable table)
	{
		m_Player = player;
		m_Table = table;
		m_Input = m_Player.GetInputController();
	}
	
	override void OnActivate()
	{
		PreAnim_CallCommand(m_Table.CMD_Action, m_Table.CMD_ActionID, 0);
	}
	
	override void OnDeactivate()
	{
		//PreAnim_CallCommand(m_Table.CMD_Action, 0, 0);
	}
	
	override bool PostPhysUpdate(float pDt)
	{
		return !m_NeedFinish;
	}
	
	void Stop()
	{
		m_NeedFinish = true;
	}
}*/