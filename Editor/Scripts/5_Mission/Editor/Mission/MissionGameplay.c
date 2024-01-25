modded class MissionGameplay
{		
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if (GetGame().GetInput().LocalPress("EditorToggleActive")) {
			GetDayZGame().GetEditor().SetActive(!GetDayZGame().GetEditor().IsActive());
		}
		
		m_HudRootWidget.Show(GetDayZGame().GetEditor() && !GetDayZGame().GetEditor().IsActive());
		
#ifdef DIAG_DEVELOPER
		DayZPlayerUtils.DrawStartFrame();
		
		for (int i = 0; i < GetDayZGame().DebugTexts.Count(); i++) {			
			DayZPlayerUtils.DrawDebugText(GetDayZGame().DebugTexts[i].param1, GetDayZGame().DebugTexts[i].param2, GetDayZGame().DebugTexts[i].param3);
		}
		
		GetDayZGame().DebugTexts.Clear();
#endif
	}
}