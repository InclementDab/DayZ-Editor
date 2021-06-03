modded class LoadingScreen
{
	static const int LOADING_SCREEN_COUNT = 16;
	
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
		
		// nice
		int val = TickCount(0);
		val = val + (val << 37);
		val = Math.AbsInt(val);
		val = val % LOADING_SCREEN_COUNT + 1;
		m_ImageWidgetBackground.LoadImageFile(0, string.Format("DayZEditor/gui/loadingscreens/%1.edds", val));
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