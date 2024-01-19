modded class MissionGameplay
{
	override void OnInit()
	{
		super.OnInit();
						
		/* Kill the weather! */
		GetGame().GetWeather().GetFog().Set(0.1, 0, 30);
		GetGame().GetWeather().GetOvercast().Set(Math.RandomFloat01() * 0.8, 0, 30);
		GetGame().GetWeather().GetRain().Set(Math.RandomFloat01() * 0.1, 0, 30);
		
		GetGame().GetWeather().SetWind(vector.Zero);
		GetGame().GetWeather().SetWindSpeed(Math.RandomFloat01() * 15.0);
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();		
		g_Game.ReportProgress("Loading Mission");
		
		CF.ObjectManager.UnhideAllMapObjects();
	}
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();		
		
		delete g_Editor;
	}
			
	override void OnMouseButtonPress(int button)
	{
		if (!GetEditor() || !GetEditor().OnMouseDown(button)) {			
			super.OnMouseButtonPress(button);
		} 
	}
}