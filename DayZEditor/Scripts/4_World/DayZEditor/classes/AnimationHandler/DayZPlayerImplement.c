modded class DayZPlayerImplement
{
	protected bool m_EditorAnimationStart;
	protected bool m_EditorAnimationStop;
	protected ref EditorAnimationManagerTable m_EditorAnimationManagerTable;

	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (super.ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished)) {
			return true;
		}
		
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT) {
			HumanCommandScript hcs = GetCommand_Script();
			EditorAnimationCommand command = EditorAnimationCommand.Cast(hcs);

			if (command) {
				if (m_EditorAnimationStop) {
					command.Stop();
				}
				
				m_EditorAnimationStop = true;
				return true;
			}
		}
		
				
		if (m_EditorAnimationStart) {
			StartCommand_Script(new EditorAnimationCommand(this, m_EditorAnimationManagerTable));
			m_EditorAnimationStart = false;
			return true;
		}
		  
		
		return false;
	}
	
	void EditorAnimationStart(string anim_name)
	{
		m_EditorAnimationManagerTable = new EditorAnimationManagerTable(this, anim_name);
		m_EditorAnimationStart = true;
		m_EditorAnimationStop = false;
	}
	
	void EditorAnimationStop()
	{
		m_EditorAnimationStart = false;
		m_EditorAnimationStop = true;
	}

	void EditorAnimationReset()
	{
		EditorAnimationStart("CMD_Jump");
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EditorAnimationStart, 50, false, "CMD_LandImmediate");
	}
}