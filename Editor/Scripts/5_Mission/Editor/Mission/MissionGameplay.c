modded class MissionGameplay
{	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		// Todo move elsewhere		
		bool player_control_enabled = GetDayZGame().GetEditor() && GetDayZGame().GetEditor().GetCurrentControl() && GetDayZGame().GetEditor().GetCurrentControl().IsInherited(Man);
		m_HudRootWidget.Show(player_control_enabled);
		
#ifdef DIAG_DEVELOPER
		DayZPlayerUtils.DrawStartFrame();
		
		for (int i = 0; i < GetDayZGame().DebugTexts.Count(); i++) {			
			DayZPlayerUtils.DrawDebugText(GetDayZGame().DebugTexts[i].param1, GetDayZGame().DebugTexts[i].param2, GetDayZGame().DebugTexts[i].param3);
		}
		
		GetDayZGame().DebugTexts.Clear();
#endif
	}
}