class EditorLoadingScreen: LoadingScreen
{
	static const ref array<ref LoadingScreenEntry> LOADING_SCREENS = {
		new LoadingScreenEntry("0.edds", "Lad"),
		new LoadingScreenEntry("1.edds", "Relict"),
		new LoadingScreenEntry("2.edds", "Suvi"), 
		new LoadingScreenEntry("3.edds", "DanceOfJesus"),
		new LoadingScreenEntry("4.edds", "Suvi"), 
		new LoadingScreenEntry("5.edds", "Lavo"),
		new LoadingScreenEntry("6.edds", "Lavo"),
		new LoadingScreenEntry("7.edds", "Kornflakes"),
		new LoadingScreenEntry("8.edds", "Boba Fetish"),
		new LoadingScreenEntry("9.edds", "Jonas"),
		new LoadingScreenEntry("10.edds", "Fluzoo"),
		//new LoadingScreenEntry("11.edds", "Mihalych"),
		new LoadingScreenEntry("12.edds", "TerrorisedCookie"),
		new LoadingScreenEntry("13.edds", "DayZ"),
		new LoadingScreenEntry("14.edds", "DanceOfJesus"),
		new LoadingScreenEntry("15.edds", "DanceOfJesus"),		
		new LoadingScreenEntry("16.edds", "ZoD1Ac"),		
		new LoadingScreenEntry("17.edds", "ollie"),		
		new LoadingScreenEntry("18.edds", "Boba Fetish"),		
		new LoadingScreenEntry("19.edds", "InclementDab"),		
		new LoadingScreenEntry("20.edds", "Bosch"),		
		new LoadingScreenEntry("21.edds", "DanceOfJesus"),		
		new LoadingScreenEntry("22.edds", "DanceOfJesus", true),		
		new LoadingScreenEntry("23.edds", "Sanches"),
		new LoadingScreenEntry("24.edds", "ItzMikeDocherty"),
		new LoadingScreenEntry("25.edds", "[H]unter"),
		new LoadingScreenEntry("26.edds", "DanceOfJesus"),	
		new LoadingScreenEntry("27.edds", "ItzMikeDocherty"),	
		new LoadingScreenEntry("28.edds", "JanceOfDesus"),	
	};
	
	protected TextWidget m_ImageCredit;
	
	void EditorLoadingScreen(DayZGame game)
	{		
		m_DayZGame = game;
		
		if (!m_DayZGame.OnProgressReport) {
			m_DayZGame.OnProgressReport = new ScriptInvoker();
		}
		
		m_DayZGame.OnProgressReport.Insert(OnGameProgressReport);
				
		m_WidgetRoot = game.GetLoadingWorkspace().CreateWidgets("DayZEditor/GUI/layouts/EditorLoadingScreen.layout");
		Class.CastTo(m_ImageLogoMid, m_WidgetRoot.FindAnyWidget("ImageLogoMid"));
		Class.CastTo(m_ImageLogoCorner, m_WidgetRoot.FindAnyWidget("ImageLogoCorner"));
		
		Class.CastTo(m_TextWidgetStatus, m_WidgetRoot.FindAnyWidget("StatusText"));
		Class.CastTo(m_ImageWidgetBackground, m_WidgetRoot.FindAnyWidget("ImageBackground"));
		Class.CastTo(m_ImageLoadingIcon, m_WidgetRoot.FindAnyWidget("ImageLoadingIcon"));
		Class.CastTo(m_ModdedWarning, m_WidgetRoot.FindAnyWidget("ModdedWarning"));
		
		m_ImageBackground = ImageWidget.Cast( m_WidgetRoot.FindAnyWidget("ImageBackground") );
		m_ProgressLoading = ProgressBarWidget.Cast( m_WidgetRoot.FindAnyWidget("LoadingBar") );

		m_ProgressText = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("ProgressText"));

		m_ModdedWarning.SetText("WARNING! The DayZ Editor is currently in BETA. Please report all bugs you find to our Discord");	
		m_ModdedWarning.Show(false);
		
		m_ImageCredit = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("ImageCredit"));
		
#ifndef COMPONENT_SYSTEM
		Math.Randomize(-1);
		int val = Math.RandomInt(0, LOADING_SCREENS.Count() * 100) % LOADING_SCREENS.Count();
		
		m_ImageWidgetBackground.LoadImageFile(0, string.Format("DayZEditor/gui/loadingscreens/%1", LOADING_SCREENS[val].File));
		m_ImageCredit.SetText("Image by " + LOADING_SCREENS[val].Creator);
		m_ImageWidgetBackground.SetImage(0);
		
		m_ImageCredit.Show(!LOADING_SCREENS[val].HideLogo);
		m_ImageLogoMid.Show(!LOADING_SCREENS[val].HideLogo);
		m_ImageLogoCorner.Show(!LOADING_SCREENS[val].HideLogo);
#endif
	}
	
	override void Show()
	{
		Widget lIcon = m_ImageBackground;
		Widget pText = m_ProgressLoading;
		m_ProgressText.Show(true);
		m_ProgressLoading.SetCurrent(0.0);
		
		if (!m_WidgetRoot.IsVisible()) {
			if (m_DayZGame.GetUIManager().IsDialogVisible()) {
				m_DayZGame.GetUIManager().HideDialog();
			}
						
			m_WidgetRoot.Show(true);
		}
		
		ProgressAsync.SetProgressData(pText);
		ProgressAsync.SetUserData(lIcon);
	}
	
	override void SetTitle(string title) {}
	
	void OnGameProgressReport(string report)
	{
		m_ProgressText.SetText(report);
	}
}