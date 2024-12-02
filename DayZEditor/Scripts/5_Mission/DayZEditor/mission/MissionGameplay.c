modded class MissionGameplay
{	
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
		GetUApi().UpdateControls();
		
		/* Kill the weather! */
		GetGame().GetWeather().GetFog().Set(0, 0, 0);
		GetGame().GetWeather().GetFog().SetLimits(0, 0);
		GetGame().GetWeather().GetOvercast().Set(0, 0, 0);
		GetGame().GetWeather().GetOvercast().SetLimits(0, 0);
		GetGame().GetWeather().GetRain().Set(0, 0, 0);
		GetGame().GetWeather().GetRain().SetLimits(0, 0);
		GetGame().GetWeather().GetSnowfall().Set(0);
		GetGame().GetWeather().GetSnowfall().SetLimits(0, 0);
		
		GetGame().GetWeather().SetWind(vector.Zero);
		GetGame().GetWeather().SetWindSpeed(0);
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
		
	override void OnMouseButtonRelease(int button)
	{
		if (!GetEditor() || !GetEditor().OnMouseRelease(button)) {
			super.OnMouseButtonRelease(button);
		}
	}

	override void OnUpdate(float timeslice)
	{
		if (GetEditor()) {
			if (GetEditor().IsPlayerControlled()) {
				super.OnUpdate(timeslice);
			}

			GetEditor().Update(timeslice);
		} else {
			super.OnUpdate(timeslice);
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
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
		// On Load unhide em all
		CF.ObjectManager.UnhideAllMapObjects();
	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		delete g_Editor;
	}
	
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		if (!GetGame().IsServer()) {
			ErrorEx("Cannot run DayZ Editor on server... exiting");
			return;
		}

		vector center_pos = Editor.GetMapCenterPosition();
		PlayerBase player = Editor.CreateDefaultCharacter(GetGame().CreateRandomPlayer(), Editor.GetSafeStartPosition(center_pos[0], center_pos[2], 500));
		if (!player) {
			Error("Player was not created, exiting");
			return;
		}

		GetGame().SelectPlayer(null, player);

		g_Editor = new Editor(player);
		g_Editor.SetActive(true);
	}
		
	override void Continue()
	{
		super.Continue();
		
		if (GetEditor().IsActive())	{
			GetEditor().GetEditorHud().Show(true);
		}
	}
}