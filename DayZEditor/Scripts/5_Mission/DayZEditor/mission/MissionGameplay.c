modded class MissionGameplay
{	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if (g_Editor && g_Editor.ShouldProcessInput()) {
			g_Editor.ProcessInput(GetGame().GetInput());
		}
	}
	
	override void OnInit()
	{
		super.OnInit();
				
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().GetInputByName("UACameraToolSpeedIncrease").ForceDisable(true);
		GetUApi().GetInputByName("UACameraToolSpeedDecrease").ForceDisable(true);
		GetUApi().GetInputByName("UAGear").ForceDisable(true);
		GetUApi().UpdateControls();
		
		/* Kill the weather! */
		GetGame().GetWeather().GetFog().Set(0, 0, 0);
		GetGame().GetWeather().GetFog().SetLimits(0, 0);
		GetGame().GetWeather().GetOvercast().Set(0, 0, 0);
		GetGame().GetWeather().GetOvercast().SetLimits(0, 0);
		GetGame().GetWeather().GetRain().Set(0, 0, 0);
		GetGame().GetWeather().GetRain().SetLimits(0, 0);
		
		GetGame().GetWeather().SetWind(vector.Zero);
		GetGame().GetWeather().SetWindSpeed(0);
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();		
		g_Game.ReportProgress("Loading Mission");
	}
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();		
		
		delete g_Editor;
	}
		
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();		
		
		g_Game.ReportProgress("Loading DayZ Editor...");
		
		g_Editor = new Editor(Editor.GenerateSafeStartPosition());
		g_Game.ReportProgress("DayZ Editor Loading complete");
	}
	
	override void Pause()
	{
		super.Pause();
		GetEditor().GetEditorHud().Show(false);
	}
	
	override void Continue()
	{
		super.Continue();		
		GetEditor().GetEditorHud().Show(GetEditor().GetCurrentControlCamera() != null);
	}
	
	override void OnKeyPress(int key)
	{
		if (!GetEditor() || !GetEditor().OnKeyPress(key)) {
			super.OnKeyPress(key);
		}	
	}
	
	override void OnKeyRelease(int key)
	{
		if (!GetEditor() || !GetEditor().OnKeyRelease(key)) {
			super.OnKeyRelease(key);
		}
	}
	
	override void OnMouseButtonPress(int button)
	{
		if (!GetEditor() || !GetEditor().OnMouseDown(button)) {			
			super.OnMouseButtonPress(button);
		} 
	}
	
	override void OnMouseButtonRelease(int button)
	{
		if (!GetEditor() || !GetEditor().OnMouseRelease(button)) {
			super.OnMouseButtonRelease(button);
		}
	}

	override void ShowInventory()
	{
		GetGame().GetPlayer().GetHumanInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		super.ShowInventory();
		m_Hud.InventoryShown = true;
	}
	
	override void HideInventory()
	{
		super.HideInventory();
		m_Hud.InventoryShown = false;
	}
}