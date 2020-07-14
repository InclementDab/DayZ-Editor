// #include "Scripts/GUI/IngameHud.c"


// creates player in world
//GetGame().SelectPlayer(null, CreateCustomDefaultCharacter());

// heading model? mouse control
//bool			HeadingModel(float pDt, SDayZPlayerHeadingModel pModel);

static PlayerBase CreateCustomDefaultCharacter()
{
    PlayerBase oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), "7500 0 7500", 0, "NONE") );

    oPlayer.GetInventory().CreateInInventory( "AviatorGlasses" );

    return oPlayer;
}


class EditorMissionServer: MissionServer
{

}


class EditorMissionGameplay: MissionGameplay
{
	static ref Editor m_Editor;
	static EditorMissionGameplay GetInstance()
	{
		return EditorMissionGameplay.Cast(GetGame().GetMission());
	}
		
    void EditorMissionGameplay()
    {
		
		Print("EditorMissionGameplay");	
    }
	

    void ~EditorMissionGameplay()
    {
        Print("~EditorMissionGameplay");
		
    }
	
	override void OnMouseButtonPress(int button)
	{
		//m_Editor.OnMouseButtonDown(button);
		super.OnMouseButtonPress(button);
		
	}

	
    override void OnKeyPress(int key)
    {
		m_Editor.OnKeyPress(key);
    	super.OnKeyPress(key);        
        
        m_Hud.KeyPress(key);
    }

    override void OnKeyRelease(int key)
    {
        super.OnKeyRelease(key);

        switch (key) {
			
            case KeyCode.KC_ESCAPE:
                PlayerBase player = GetGame().GetPlayer();
                break;
			case KeyCode.KC_DOWN:
				delete m_Editor;
				m_Editor = new Editor(); 
				break;


            default:
				break;
                //Print("OnKeyRelease " + key);
        }
    }



    
    override void OnInit()
    {
		Print("EditorMissionGameplay::OnInit");
        if (m_Initialized)
        {
            return;
        }
		
		

        PPEffects.Init();
        MapMarkerTypes.Init();

        m_UIManager = GetGame().GetUIManager();
        m_Initialized = true;

        // init hud ui
        if (!m_HudRootWidget)
        {
            m_HudRootWidget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_hud.layout");
            m_HudRootWidget.Show(false);
			
            m_Chat.Init(m_HudRootWidget.FindAnyWidget("ChatFrameWidget"));
            m_ActionMenu.Init(m_HudRootWidget.FindAnyWidget("ActionsPanel"), TextWidget.Cast(m_HudRootWidget.FindAnyWidget("DefaultActionWidget")));
            m_Hud.Init(m_HudRootWidget.FindAnyWidget("HudPanel"));

            // von enabled icon
            m_MicrophoneIcon = ImageWidget.Cast(m_HudRootWidget.FindAnyWidget("mic"));
            m_MicrophoneIcon.Show(false);

            // von voice level
            m_VoiceLevels = m_HudRootWidget.FindAnyWidget("VoiceLevelsPanel");
            m_VoiceLevelsWidgets = new map<int, ImageWidget>;       // [key] voice level
            m_VoiceLevelTimers = new map<int, ref WidgetFadeTimer>; // [key] voice level

            if (m_VoiceLevels)
            {
                m_VoiceLevelsWidgets.Set(VoiceLevelWhisper, ImageWidget.Cast(m_VoiceLevels.FindAnyWidget("Whisper")));
                m_VoiceLevelsWidgets.Set(VoiceLevelTalk, ImageWidget.Cast(m_VoiceLevels.FindAnyWidget("Talk")));
                m_VoiceLevelsWidgets.Set(VoiceLevelShout, ImageWidget.Cast(m_VoiceLevels.FindAnyWidget("Shout")));

                m_VoiceLevelTimers.Set(VoiceLevelWhisper, new WidgetFadeTimer);
                m_VoiceLevelTimers.Set(VoiceLevelTalk, new WidgetFadeTimer);
                m_VoiceLevelTimers.Set(VoiceLevelShout, new WidgetFadeTimer);
            }

            HideVoiceLevelWidgets();

            // chat channel
            m_ChatChannelArea = m_HudRootWidget.FindAnyWidget("ChatChannelPanel");
            m_ChatChannelText = TextWidget.Cast(m_HudRootWidget.FindAnyWidget("ChatChannelText"));
        }

        // init hud ui

#ifdef DEBUG
        
            m_HudDebug = new HudDebug;
            if (!m_HudDebug.IsInitialized()) {
                m_HudDebug.Init(GetGame().GetWorkspace().CreateWidgets("gui/layouts/debug/day_z_hud_debug.layout"));

                Debug.SetEnabledLogs(PluginConfigDebugProfile.GetInstance().GetLogsEnabled());
            }
#endif

        //AIBehaviourHL.RegAIBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);
        //RegBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);

        if (GetGame().IsMultiplayer())
        {
            OnlineServices.m_MuteUpdateAsyncInvoker.Insert(SendMuteListToServer);
        }
    }


    override void OnMissionStart()
    {
		Print("EditorMissionGameplay::OnMissionStart");		
		m_Editor = new Editor(); 
		super.OnMissionStart();
    }

    override void OnMissionFinish()
    {
        Print("EditorMissionGameplay::OnMissionFinish");
		delete m_Editor;
		super.OnMissionFinish();

    }

    


 

    override void PlayerControlEnable(bool bForceSupress)
    {
		Print("EditorMissionGameplay::PlayerControlEnable");		
        super.PlayerControlEnable(bForceSupress);

        GetUApi().GetInputByName("UAWalkRunTemp").ForceEnable(false); // force walk off!
        GetUApi().UpdateControls();

        // supress control for next frame
        GetUApi().SupressNextFrame(bForceSupress);

        m_ControlDisabled = false;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        HumanInputController hic = player.GetInputController();
        hic.LimitsDisableSprint(false);
    }

    //!movement restrictions
    override void PlayerControlDisable(int mode)
    {
		Print("EditorMissionGameplay::PlayerControlDisable");	
        super.PlayerControlDisable(mode);

        switch (mode)
        {
	        case INPUT_EXCLUDE_ALL:
	        {
	            GetUApi().ActivateExclude("menu");
	            break;
	        }
	        case INPUT_EXCLUDE_INVENTORY:
	        {
	            GetUApi().ActivateExclude("inventory");
	            GetUApi().GetInputByName("UAWalkRunTemp").ForceEnable(true); // force walk on!
	            break;
	        }
	        case INPUT_EXCLUDE_MOUSE_ALL:
	        {
	            GetUApi().ActivateExclude("radialmenu");
	            break;
	        }
	        case INPUT_EXCLUDE_MOUSE_RADIAL:
	        {
	            GetUApi().ActivateExclude("radialmenu");
	            break;
	        }
        }

        GetUApi().UpdateControls();
        m_ControlDisabled = true;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
        {
            HumanInputController hic = player.GetInputController();
            //hic.ResetADS();
            player.RequestResetADSSync();
        }
    }

    

   
    override void ShowInventory()
    {
        UIScriptedMenu menu = GetUIManager().GetMenu();

        if (!menu && GetGame().GetPlayer().GetHumanInventory().CanOpenInventory() && !GetGame().GetPlayer().IsInventorySoftLocked() && GetGame().GetPlayer().GetHumanInventory().IsInventoryUnlocked())
        {
            if (!m_InventoryMenu)
            {
                InitInventory();
            }

            if (!GetUIManager().FindMenu(MENU_INVENTORY))
            {
                GetUIManager().ShowScriptedMenu(m_InventoryMenu, null);
                PlayerBase.Cast(GetGame().GetPlayer()).OnInventoryMenuOpen();
            }
            MoveHudForInventory(true);
            PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);
        }
    }

    override void HideInventory()
    {
        if (m_InventoryMenu)
        {
            GetUIManager().HideScriptedMenu(m_InventoryMenu);
            MoveHudForInventory(false);
            PlayerControlEnable(false);
            PlayerBase.Cast(GetGame().GetPlayer()).OnInventoryMenuClose();
            VicinityItemManager.GetInstance().ResetRefreshCounter();
        }
    }

    void DestroyInventory()
    {
        if (m_InventoryMenu)
        {
            m_InventoryMenu.Close();
            m_InventoryMenu = NULL;
        }
    }

    override void ResetGUI()
    {
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

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player && !player.IsPlayerLoaded() || IsPlayerRespawning())
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
        {
            return;
        }

        PlayerControlEnable(true);
        GetUIManager().CloseMenu(MENU_INGAME);
        //GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CloseInGameMenu,1,true);
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


Mission EditorCreateCustomMission(string path)
{
	Print("DayZEditorGameplay::CreateCustomMission " + path);
	
	return new EditorMissionGameplay();
}


static void ResetMission()
{
	GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().RestartMission);
}

modded class DayZIntroScene
{
	protected Object m_FunnyMeme;
	
	void DayZIntroScene()
	{
		
		
		
		delete m_Character;
		m_FunnyMeme = GetGame().CreateObject("DSLRCamera", m_CharacterPos, true);
		m_FunnyMeme.SetOrientation(m_CharacterRot);
	}
}


modded class MainMenu 
{
	override Widget Init()
	{
		super.Init();
		
		m_ChooseServer.Show(false);
		m_CustomizeCharacter.Show(false);
		m_Stats.HideStats();
		
		Widget c = layoutRoot.FindAnyWidget("character");
		c.Show(false);
		
		TextWidget tw = TextWidget.Cast(layoutRoot.FindAnyWidget("play_label"));
		tw.SetText("Open Editor");
		
		
		return layoutRoot;
	}
	
	override void Play()
	{
		GetGame().PlayMission("missions\\DayZEditor.ChernarusPlus");
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if(IsFocusable(w)) {
			ColorHighlight( w );
			return true;
		}
		return false;
	}
}