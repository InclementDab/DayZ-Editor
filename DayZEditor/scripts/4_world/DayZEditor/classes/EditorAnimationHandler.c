
class EditorAnimationManagerTable
{
	int CMD_Action;
	int CMD_ActionID;
	
	void EditorAnimationManagerTable(Human human, string anim_name)
	{
		HumanAnimInterface anim_interface = human.GetAnimInterface();
		
		ref array<string> anim_split = {};
		
		anim_name.Split(",", anim_split);
		CMD_Action = anim_interface.BindCommand(anim_split[0]);
		
		if (anim_split.Count() > 1) {
			CMD_ActionID = anim_split[1].ToInt();
		}
	}
	
}

class EditorAnimationCommand: HumanCommandScript
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
}


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
		thread _EditorAnimationReset();
	}
	
	private void _EditorAnimationReset()
	{
		EditorAnimationStart("CMD_Jump");
		Sleep(50);
		EditorAnimationStart("CMD_LandImmediate");
	}
}



