// #include "Scripts/GUI/IngameHud.c"




Mission CreateCustomMission(string path)
{
	Print("DayZEditorGameplay::CreateCustomMission " + path);
	return new MissionGameplay();
}



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
		GetUApi().UpdateControls();
		
		//m_HudRootWidget.Show(false);
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
		m_EditorInstance = new Editor(); 
	}
	
	
	void HandleInputs(Input input)
	{
		
		if (input.LocalPress("UAUIMenu", false) || input.LocalPress("UAUIBack", false)) {
			
			if (GetEditor().GetUIManager().IsModalActive()) {
				GetEditor().GetUIManager().ModalClose();
				return;
			} 
			
			/*
			if (GetFocus() != null) {
				SetFocus(null);
				return;
			}*/
			
			
			if (GetEditor().IsLootEditActive()) {
				GetEditor().PlaceholderRemoveLootMode();
				return;
			}
		

			if (IsPaused() && !g_Game.GetUIManager().ScreenFadeVisible()) {
				Continue();
				return;
			}
			
			if (!IsPaused()) {
				Pause();
				return;
			}	
		}
		
		
		if (input.LocalPress("EditorToggleUI")) {
			
			if (input.LocalValue("UAWalkRunTemp"))
				m_EditorInstance.Redo();
			else 
				m_EditorInstance.GetUIManager().SetVisibility(!m_EditorInstance.GetUIManager().GetVisibility());
			
			
			return;
		} 
		
		if (input.LocalPress("EditorToggleCursor")) {
			if (GetGame().GetUIManager().IsCursorVisible() && !m_EditorInstance.GetUIManager().GetEditorUI().IsMapOpen()) {
				m_EditorInstance.GetUIManager().HideCursor();
				if (Editor.IsPlayerActive())
					Editor.SetPlayerAimLock(false);
				return;
			} 
			
			
			m_EditorInstance.GetUIManager().ShowCursor();
			if (Editor.IsPlayerActive())
				Editor.SetPlayerAimLock(true); //PlayerControlEnable(true); ????
			return;
		} 
		
		
	}
	
	override void OnUpdate(float timeslice)
	{
		Input input = GetGame().GetInput();
		
		HandleInputs(input);
		
		//super.OnUpdate(timeslice);
		
	}
	
	override void OnKeyPress(int key)
    {
		Input input = GetGame().GetInput();
		
		switch (key) {
			
			case KeyCode.KC_F5: {
				m_HudRootWidget.Show(true);
				break;
			}
			
			case KeyCode.KC_F6: {
				m_HudRootWidget.Show(false);
				break;
			}
		}

		m_Hud.KeyPress(key);
		if (m_EditorInstance.OnKeyPress(key)) return;
		super.OnKeyPress(key);
    }
	
	override bool IsPaused()
    {
        return GetGame().GetUIManager().IsMenuOpen(MENU_INGAME);
    }

    override void Pause()
    {
        if (IsPaused() || (GetGame().GetUIManager().GetMenu() && GetGame().GetUIManager().GetMenu().GetID() == MENU_INGAME))
            return;

        if (g_Game.IsClient() && g_Game.GetGameState() != DayZGameState.IN_GAME)
            return;


        CloseAllMenus();

        // open ingame menu
        GetUIManager().EnterScriptedMenu(MENU_INGAME, GetGame().GetUIManager().GetMenu());
        PlayerControlDisable(INPUT_EXCLUDE_ALL);
    }
	
	override void Continue()
    {
        int menu_id = GetGame().GetUIManager().GetMenu().GetID();
        if (!IsPaused() || (menu_id != MENU_INGAME && menu_id != MENU_LOGOUT) || (m_Logout && m_Logout.layoutRoot.IsVisible()))
            return;
        
        PlayerControlEnable(true);
        GetUIManager().CloseMenu(MENU_INGAME);
		
		EditorUI ui = new EditorUI();
		GetUIManager().ShowScriptedMenu(ui, GetGame().GetUIManager().GetMenu());
		GetEditor().GetUIManager().SetEditorUI(ui);
    }
	
	
}

class EditorMissionGameplay: MissionGameplay
{

    
	

    override void DestroyInventory()
    {
        if (m_InventoryMenu) {
            m_InventoryMenu.Close();
            m_InventoryMenu = NULL;
        }
    }

    override void ResetGUI()
    {
		Print("EditorMissionGameplay::ResetGUI");
        DestroyInventory();
        InitInventory();
    }

    override void ShowChat()
    {
        m_ChatChannelHideTimer.Stop();
        m_ChatChannelFadeTimer.Stop();
        m_ChatChannelArea.Show(false);
        m_UIManager.EnterScriptedMenu(MENU_CHAT_INPUT, NULL);

        int level = GetGame().GetVoiceLevel();
        UpdateVoiceLevelWidgets(level);

        PlayerControlDisable(INPUT_EXCLUDE_ALL);
    }

    override void RefreshCrosshairVisibility()
    {
        GetHudDebug().RefreshCrosshairVisibility();
    }

    override void HideCrosshairVisibility()
    {
        GetHudDebug().HideCrosshairVisibility();
    }

    



   
    override void AbortMission()
    {
#ifdef BULDOZER
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game.RequestExit, IDC_MAIN_QUIT);
#else
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().AbortMission);
#endif
    }

    override void CreateLogoutMenu(UIMenuPanel parent)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

        // do not show logout screen if player's dead
        if (!player || player.IsDamageDestroyed())
        {
            // exit the game immediately
            AbortMission();
            return;
        }

        if (parent)
        {
            m_Logout = LogoutMenu.Cast(parent.EnterScriptedMenu(MENU_LOGOUT));

            if (m_Logout)
            {
                m_Logout.SetLogoutTime();
            }
        }
    }

    override void StartLogoutMenu(int time)
    {
        if (m_Logout)
        {
            if (time > 0)
            {
                // character will be deleted from server int "time" seconds
                m_Logout.SetTime(time);
                m_Logout.Show();

                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_Logout.UpdateTime, 1000, true);
            }
            else
            {
                // no waiting time -> player is most likely dead
                m_Logout.Exit();
            }
        }
    }

    override void CreateDebugMonitor()
    {
        if (!m_DebugMonitor)
        {
            m_DebugMonitor = new DebugMonitor();
            m_DebugMonitor.Init();
        }
    }


    override void UpdateVoiceLevelWidgets(int level)
    {
        for (int n = 0; n < m_VoiceLevelsWidgets.Count(); n++)
        {
            int voiceKey = m_VoiceLevelsWidgets.GetKey(n);
            ImageWidget voiceWidget = m_VoiceLevelsWidgets.Get(n);

            // stop fade timer since it will be refreshed
            ref WidgetFadeTimer timer = m_VoiceLevelTimers.Get(n);
            timer.Stop();

            // show widgets according to the level
            if (voiceKey <= level)
            {
                voiceWidget.SetAlpha(1.0); // reset from possible previous fade out
                voiceWidget.Show(true);

                if (!m_VoNActive && !GetUIManager().FindMenu(MENU_CHAT_INPUT))
                    timer.FadeOut(voiceWidget, 3.0);
            }
            else
                voiceWidget.Show(false);
        }

        // fade out microphone icon when switching levels without von on
        if (!m_VoNActive)
        {
            if (!GetUIManager().FindMenu(MENU_CHAT_INPUT))
            {
                m_MicrophoneIcon.SetAlpha(1.0);
                m_MicrophoneIcon.Show(true);

                m_MicFadeTimer.FadeOut(m_MicrophoneIcon, 3.0);
            }
        }
        else
        {
            // stop mic icon fade timer when von is activated
            m_MicFadeTimer.Stop();
        }
    }
    override bool IsVoNActive()
    {
        return m_VoNActive;
    }

    override void HideVoiceLevelWidgets()
    {
        for (int n = 0; n < m_VoiceLevelsWidgets.Count(); n++)
        {
            ImageWidget voiceWidget = m_VoiceLevelsWidgets.Get(n);
            voiceWidget.Show(false);
        }
    }

    override UIScriptedMenu GetNoteMenu()
    {
        return m_Note;
    };

    override void SetNoteMenu(UIScriptedMenu menu)
    {
        m_Note = NoteMenu.Cast(menu);
    };

    override void SetPlayerRespawning(bool state)
    {
        m_PlayerRespawning = state;
    }

    override bool IsPlayerRespawning()
    {
        return m_PlayerRespawning;
    }
}





static void ResetMission()
{
	GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().RestartMission);
}


