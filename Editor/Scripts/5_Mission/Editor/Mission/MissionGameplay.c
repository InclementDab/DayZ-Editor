modded class MissionGameplay
{	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		// Todo move elsewhere		
		bool player_control_enabled = GetDayZGame().GetEditor() && GetDayZGame().GetEditor().GetCurrentControl() && GetDayZGame().GetEditor().GetCurrentControl().IsInherited(Man);
		m_HudRootWidget.Show(player_control_enabled);
	}
}