class ER7Scope: Rangefinder
{
	override void StartPeriodicMeasurement()
	{
		if (!m_Timer) {
			m_Timer = new Timer(CALL_CATEGORY_GAMEPLAY);
		}
		
		m_RangeText = TextWidget.Cast(GetGame().GetWorkspace().CreateWidgets( "gui/layouts/gameplay/rangefinder_hud.layout"));
		
		m_Timer.Run(GetMeasurementUpdateInterval(), this, "DoMeasurement", null, true);
	}
	
	override void StopPeriodicMeasurement()
	{
		if (m_Timer) {
			m_Timer.Stop();
		}
		
		if (m_RangeText) {
			delete m_RangeText;
		}
	}
}