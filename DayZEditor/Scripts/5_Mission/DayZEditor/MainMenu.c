modded class MainMenu 
{
	protected ref EditorMainMenuStats m_EditorMainMenuStats;
	protected Widget m_JoinDiscord;
	protected Widget m_OpenWiki;
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/GUI/layouts/EditorMainMenu.layout");
		
		m_Play						= layoutRoot.FindAnyWidget("play");
		m_ChooseServer				= layoutRoot.FindAnyWidget("donate");
		m_CustomizeCharacter		= layoutRoot.FindAnyWidget("customize_character");
		m_PlayVideo					= layoutRoot.FindAnyWidget("play_video");
		m_Tutorials					= layoutRoot.FindAnyWidget("tutorials");
		m_TutorialButton			= layoutRoot.FindAnyWidget("tutorial_button");
		m_MessageButton				= layoutRoot.FindAnyWidget("message_button");
		m_SettingsButton			= layoutRoot.FindAnyWidget("settings_button");
		m_Exit						= layoutRoot.FindAnyWidget("exit_button");
		m_PrevCharacter				= layoutRoot.FindAnyWidget("prev_character");
		m_NextCharacter				= layoutRoot.FindAnyWidget("next_character");
		m_JoinDiscord				= layoutRoot.FindAnyWidget("discord");
		m_OpenWiki					= layoutRoot.FindAnyWidget("wiki");

		m_Version					= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		m_ModdedWarning				= TextWidget.Cast( layoutRoot.FindAnyWidget( "ModdedWarning" ) );
		m_CharacterRotationFrame	= layoutRoot.FindAnyWidget( "character_rotation_frame" );
		
		m_LastPlayedTooltip			= layoutRoot.FindAnyWidget( "last_server_info" );
		m_LastPlayedTooltip.Show(false);
		m_LastPlayedTooltipLabel	= m_LastPlayedTooltip.FindAnyWidget( "last_server_info_label" );
		m_LastPlayedTooltipName 	= TextWidget.Cast( m_LastPlayedTooltip.FindAnyWidget( "last_server_info_name" ) );
		m_LastPlayedTooltipIP		= TextWidget.Cast( m_LastPlayedTooltip.FindAnyWidget( "last_server_info_ip" ) );
		m_LastPlayedTooltipPort		= TextWidget.Cast( m_LastPlayedTooltip.FindAnyWidget( "last_server_info_port" ) );
		
		m_LastPlayedTooltipTimer	= new WidgetFadeTimer();
		
		m_EditorMainMenuStats		= new EditorMainMenuStats( layoutRoot.FindAnyWidget( "character_stats_root" ) );
		
		m_Mission					= MissionMainMenu.Cast( GetGame().GetMission() );
		
		m_LastFocusedButton = 		m_Play;

		m_ScenePC					= m_Mission.GetIntroScenePC();
		
		//if (m_ScenePC) {
		//	m_ScenePC.ResetIntroCamera();
		//}
		
		
		m_PlayerName				= TextWidget.Cast( layoutRoot.FindAnyWidget("character_name_text") );
			
		GetGame().GetUIManager().ScreenFadeOut(0);

		SetFocus( null );
		
		Refresh();
		
		LoadMods();
		
		GetDayZGame().GetBacklit().MainMenu_OnShow();
	
		g_Game.SetLoadState( DayZLoadState.MAIN_MENU_CONTROLLER_SELECT );
		
		string version;
		GetGame().GetVersion(version);
		m_Version.SetText(string.Format("#main_menu_version %1 - #STR_EDITOR_MAIN_MENU_VERSION %2", version, GetEditor().Version));
		
		return layoutRoot;
	}
	
    override void Play()
    {
		thread CreateLoadDialog();
    }
	
	// Copy pasted from EditorLoadMapCommand
	// but since editor isnt loaded, cant run it
	private void CreateLoadDialog()
	{
		EditorMapSelectDialog select_dialog = new EditorMapSelectDialog("Select Map...");
		string selected_map;
		DialogResult result = select_dialog.ShowDialog(selected_map);
		
		if (selected_map != string.Empty && result == DialogResult.OK) {
			GetGame().PlayMission(CreateEditorMission(selected_map));
		}
	}

    override bool OnMouseEnter(Widget w, int x, int y)
    {
		if (IsFocusable(w)) {
			ColorHighlight(w);
			return true;
		}
		
		return false;
	}
	
	override bool IsFocusable( Widget w )
	{
		if ( w )
		{
			if ( w == m_Play || w == m_ChooseServer || w == m_CustomizeCharacter || w == m_TutorialButton || w == m_MessageButton || w == m_SettingsButton );
			{
				return true;
			}
			
			if ( w == m_Exit || w == m_PlayVideo );
			{
				return true;
			}
			
			if ( w == m_NewsMain || w == m_NewsSec1 || w == m_NewsSec2 || w == m_PrevCharacter || w == m_NextCharacter || w == m_JoinDiscord);
			{
				return true;
			}
		}
		
		return false;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if (button == MouseState.LEFT) {
			switch (w) {
				case m_JoinDiscord: {
					m_LastFocusedButton = m_JoinDiscord;
					GetGame().OpenURL("discord.com/invite/5g742yH");
					return true;
				}
				
				case m_OpenWiki: {
					m_LastFocusedButton = m_OpenWiki;
					GetGame().OpenURL("github.com/InclementDab/DayZ-Editor/wiki");
					return true;
				}
			}
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	override void OnChangeCharacter(bool create_character = true)
	{
		super.OnChangeCharacter(create_character);
		m_EditorMainMenuStats.UpdateStats();
	}
	
	override void OpenMenuServerBrowser()
	{
		GetGame().OpenURL("paypal.com/donate/?business=YPCJSENU7QZTA&currency_code=USD");
	}
	
	
}