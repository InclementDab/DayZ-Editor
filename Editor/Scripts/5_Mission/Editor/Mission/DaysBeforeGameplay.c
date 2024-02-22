modded class DaysBeforeGameplay
{		
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
				
#ifdef DIAG_DEVELOPER
		DayZPlayerUtils.DrawStartFrame();
		
		for (int i = 0; i < GetDayZGame().DebugTexts.Count(); i++) {			
			DayZPlayerUtils.DrawDebugText(GetDayZGame().DebugTexts[i].param1, GetDayZGame().DebugTexts[i].param2, GetDayZGame().DebugTexts[i].param3);
		}
		
		GetDayZGame().DebugTexts.Clear();
#endif
	}
}