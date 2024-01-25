/*modded class MissionMainMenu
{
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		if (m_IntroScenePC)
	    	m_IntroScenePC.OnUpdate(timeslice);
	}
	
	override void PlayMusic()
	{
		if (Editor.GetCurrentHoliday() == EditorHoliday.CHRISTMAS || Editor.GetCurrentHoliday() == EditorHoliday.NEWYEARS) {
			return;
		}
		
		super.PlayMusic();
	}
}*/