modded class MissionGameplay
{	
	protected ref EditorMainMenu m_PauseMenu;
	
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
		
		DayZGame.Event_OnRPC.Insert(OnERPC);
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
			if (GetEditor().GetControllingPlayer() && !GetEditor().IsActive()) {
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
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		delete g_Editor;
	}
	
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		/*
		if (!GetGame().IsServer()) {
			ErrorEx("Cannot run DayZ Editor on server... exiting");
			return;
		}*/

		vector center_pos = Editor.GetMapCenterPosition();
		vector start_pos = Editor.GetSafeStartPosition(center_pos[0], center_pos[2], 3500);
		PlayerBase player = Editor.CreateDefaultCharacter(GetGame().CreateRandomPlayer(), start_pos);
		if (!player) {
			Error("Player was not created, exiting");
			return;
		}

		// Make sure to select player immediately so they can be controlled
		GetGame().SelectPlayer(null, player);

		g_Editor = new Editor(player);
		g_Editor.SetActive(true);
	}
		
	/*
	override void Continue()
	{
		super.Continue();
		
		if (GetEditor().IsActive())	{
			GetEditor().GetEditorHud().Show(true);
		}
	}*/
	
	override bool IsPaused()
	{
		return m_PauseMenu != null;
	}
	
	override void Pause()
	{
		if (IsPaused() || m_PauseMenu)
		{
			return;
		}

		m_PauseQueued = true;

		if (g_Game.GetGameState() != DayZGameState.IN_GAME) {
			return;
		}
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player && !player.IsPlayerLoaded() || IsPlayerRespawning()) {
			return;
		}
		
		CloseAllMenus();
		
		// open ingame menu
		m_PauseMenu = new EditorMainMenu();

		AddActiveInputExcludes({"menu"});
		AddActiveInputRestriction(EInputRestrictors.INVENTORY);

		m_PauseQueued = false;
	}
	
	override void Continue()
	{
		//GetGame().GetUIManager().Back();
		
		RemoveActiveInputExcludes({"menu"},true);
		RemoveActiveInputRestriction(EInputRestrictors.INVENTORY);
		delete m_PauseMenu;
			
		if (GetEditor().IsActive())	{
			GetEditor().GetEditorHud().Show(true);
		}
	}
	
	void OnERPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		switch (rpc_type) {
            case 39252: {
				if (GetGame().IsMultiplayer()) {
                	EditorObjectData dta = new EditorObjectData();
					string uuid;
					ctx.Read(uuid);
                	dta.Read(ctx, int.MAX);
					
					GetEditor().GetObjectManager().CreateObject(uuid, dta);
				}
				
                break;
            }
			
			case 39253: {
				if (GetGame().IsMultiplayer()) {
					string uuid2;
					ctx.Read(uuid2);					
					GetEditor().GetObjectManager().DeleteObject(uuid2);
				}
				
                break;
            }
			
			case 39254: {
				if (GetGame().IsMultiplayer()) {
					string uuid3;
					ctx.Read(uuid3);			
					EditorObjectData dta2 = new EditorObjectData();		
					dta2.Read(ctx, int.MAX);
					
					GetEditor().GetObjectManager().UpdateObject(uuid3, dta2);
				}
				
                break;
            }
        }
    } 
}