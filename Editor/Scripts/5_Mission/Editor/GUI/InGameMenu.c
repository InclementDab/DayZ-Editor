modded class InGameMenu
{	
	override Widget Init()
	{		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("Editor/GUI/layouts/EditorPauseMenu.layout");
				
		m_ContinueButton			= layoutRoot.FindAnyWidget( "continuebtn" );
		m_ExitButton				= layoutRoot.FindAnyWidget( "exitbtn" );
		m_RestartButton				= layoutRoot.FindAnyWidget( "restartbtn" );
		m_RespawnButton 			= layoutRoot.FindAnyWidget( "respawn_button" );
		m_RestartDeadRandomButton	= layoutRoot.FindAnyWidget( "respawn_button_random" );
		m_RestartDeadCustomButton	= layoutRoot.FindAnyWidget( "respawn_button_custom" );
		m_OptionsButton				= layoutRoot.FindAnyWidget( "optionsbtn" );
		m_ModdedWarning				= TextWidget.Cast( layoutRoot.FindAnyWidget( "ModdedWarning" ) );
		m_HintPanel					= new UiHintPanel(layoutRoot.FindAnyWidget( "hint_frame" ));
		m_ServerInfoPanel 			= layoutRoot.FindAnyWidget( "server_info" );
		m_ServerIP 					= TextWidget.Cast( layoutRoot.FindAnyWidget( "server_ip" ) );
		m_ServerPort 				= TextWidget.Cast( layoutRoot.FindAnyWidget( "server_port" ) );
		m_ServerName 				= TextWidget.Cast( layoutRoot.FindAnyWidget( "server_name" ) );
		//m_FavoriteButton 			= layoutRoot.FindAnyWidget( "favorite_button" );
		m_FavoriteImage 			= layoutRoot.FindAnyWidget( "favorite_image" );
		m_UnfavoriteImage 			= layoutRoot.FindAnyWidget( "unfavorite_image" );
		m_CopyInfoButton 			= layoutRoot.FindAnyWidget( "copy_button" );
		
		if (GetGame().IsMultiplayer()) {
			m_RestartButton.Show(false);
		}
				
		HudShow( false );
		SetGameVersion();

		return layoutRoot;
	}
	
	protected override void SetGameVersion()
	{
		TextWidget version_widget = TextWidget.Cast(layoutRoot.FindAnyWidget("version"));		
		string version;
		GetGame().GetVersion(version);
		//version_widget.SetText(string.Format("#main_menu_version %1 - #STR_EDITOR_MAIN_MENU_VERSION %2", version, GetDayZGame().GetEditor().Version));
	}
	
	override void UpdateGUI() 
	{
		if (!IsMissionOffline())
			super.UpdateGUI();
	}
	
	override void OnClick_Restart()
	{
		if (!IsMissionOffline())
			return;
		
		//GetDayZGame().GetEditor().CommandManager[EditorLoadMapCommand].Execute(this, null);
	}
	
	override void OnClick_Respawn()
	{
		if (!GetDayZGame().GetEditor()) {
			super.OnClick_Respawn();
			return;
		}
		
		//GetDayZGame().GetEditor().CommandManager[EditorOpenCommand].Execute(this, null);
	}
	
	override void OnClick_Exit()
	{
		if (!GetDayZGame().GetEditor()) {
			super.OnClick_Exit();
			return;
		}
		
		//GetDayZGame().GetEditor().CommandManager[EditorExitCommand].Execute(this, null);
	}
}