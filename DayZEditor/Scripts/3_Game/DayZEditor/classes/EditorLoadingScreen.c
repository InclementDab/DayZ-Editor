// filename, author
typedef Param2<string, string> LoadingScreenEntry;

modded class LoadingScreen
{
	static const ref array<ref LoadingScreenEntry> LOADING_SCREENS = {
		new LoadingScreenEntry("0.edds", "Lad"),
		new LoadingScreenEntry("1.edds", "Unknown"),
		new LoadingScreenEntry("2.edds", "Suvi"), 
		new LoadingScreenEntry("3.edds", "DanceOfJesus"),
		new LoadingScreenEntry("4.edds", "Suvi"), 
		new LoadingScreenEntry("5.edds", "Lavo"),
		new LoadingScreenEntry("6.edds", "Lavo"),
		new LoadingScreenEntry("7.edds", "Unknown"),
		new LoadingScreenEntry("8.edds", "Boba Fetish"),
		new LoadingScreenEntry("9.edds", "Jonas"),
		new LoadingScreenEntry("10.edds", "Fluzoo"),
		new LoadingScreenEntry("11.edds", "Mihalych"),
		new LoadingScreenEntry("12.edds", "TerrorisedCookie"),
		new LoadingScreenEntry("13.edds", "DayZ"),
		new LoadingScreenEntry("14.edds", "DanceOfJesus"),
		new LoadingScreenEntry("15.edds", "DanceOfJesus"),		
		new LoadingScreenEntry("16.edds", "ZoD1Ac"),		
		new LoadingScreenEntry("17.edds", "ollie"),		
	};
	
	protected TextWidget m_ImageCredit;
	
	void LoadingScreen(DayZGame game)
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
		
		// nice
		int val = TickCount(0);
		val = val + (val << 37);
		val = Math.AbsInt(val);
		val = val % LOADING_SCREENS.Count();
		m_ImageWidgetBackground.LoadImageFile(0, string.Format("DayZEditor/gui/loadingscreens/%1", LOADING_SCREENS[val].param1));
		m_ImageCredit.SetText("Image by " + LOADING_SCREENS[val].param2);
		m_ImageWidgetBackground.SetImage(0);
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