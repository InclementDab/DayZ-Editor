class EditorLoadingScreen: LoadingScreen
{
	static const ref array<ref EditorLoadingScreenEntry> LOADING_SCREENS = {
		new EditorLoadingScreenEntry("0.edds", "Lad"),
		new EditorLoadingScreenEntry("1.edds", "Relict"),
		new EditorLoadingScreenEntry("2.edds", "Suvi"), 
		new EditorLoadingScreenEntry("3.edds", "DanceOfJesus"),
		new EditorLoadingScreenEntry("4.edds", "Suvi"), 
		new EditorLoadingScreenEntry("5.edds", "Lavo"),
		new EditorLoadingScreenEntry("6.edds", "Lavo"),
		new EditorLoadingScreenEntry("7.edds", "Kornflakes"),
		new EditorLoadingScreenEntry("8.edds", "Boba Fetish"),
		new EditorLoadingScreenEntry("9.edds", "Jonas"),
		new EditorLoadingScreenEntry("10.edds", "Fluzoo"),
		//new EditorLoadingScreenEntry("11.edds", "Mihalych"),
		//new EditorLoadingScreenEntry("12.edds", "TerrorisedCookie"),
		new EditorLoadingScreenEntry("13.edds", "DayZ"),
		new EditorLoadingScreenEntry("14.edds", "DanceOfJesus"),
		new EditorLoadingScreenEntry("15.edds", "DanceOfJesus"),		
		new EditorLoadingScreenEntry("16.edds", "ZoD1Ac"),		
		new EditorLoadingScreenEntry("17.edds", "ollie"),		
		new EditorLoadingScreenEntry("18.edds", "Boba Fetish"),		
		new EditorLoadingScreenEntry("19.edds", "InclementDab"),		
		new EditorLoadingScreenEntry("20.edds", "Bosch"),		
		new EditorLoadingScreenEntry("21.edds", "DanceOfJesus"),		
		new EditorLoadingScreenEntry("22.edds", "DanceOfJesus", true),		
		new EditorLoadingScreenEntry("23.edds", "Sanches"),
		new EditorLoadingScreenEntry("24.edds", "ItzMikeDocherty"),
		new EditorLoadingScreenEntry("25.edds", "[H]unter"),
		new EditorLoadingScreenEntry("26.edds", "DanceOfJesus"),	
		new EditorLoadingScreenEntry("27.edds", "ItzMikeDocherty"),	
		new EditorLoadingScreenEntry("28.edds", "JanceOfDesus"),	
		new EditorLoadingScreenEntry("29.edds", "InclementDab"),	
		new EditorLoadingScreenEntry("30.edds", "Welliton"),	
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
		
		m_ImageWidgetBackground.LoadImageFile(0, string.Format("DayZEditor/gui/loadingscreens/%1", LOADING_SCREENS[val].Image));
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