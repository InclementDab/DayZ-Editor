class EditorLoadingScreen: LoadingScreen
{
	static const ref array<ref EditorLoadingScreenEntry> LOADING_SCREENS = {
		new EditorLoadingScreenEntry("attla68522-1727105371.edds", "Attla68522"),
		new EditorLoadingScreenEntry("campino1337-1727104641.edds", "Campino1337"),
		new EditorLoadingScreenEntry("impipapo-1727267576.edds", "VVils0on"), 
		new EditorLoadingScreenEntry("itzmikedocherty-1727104663.edds", "ItzMikeDocherty"),
		new EditorLoadingScreenEntry("itzmikedocherty-1727104768.edds", "ItzMikeDocherty"), 
		new EditorLoadingScreenEntry("kowalskiz-1727301225.edds", "Kowalskiz"),
		new EditorLoadingScreenEntry("lunna2233-1727117795.edds", "Lunna2233"),
		new EditorLoadingScreenEntry("lunna2233-1727808367.edds", "Lunna2233"),
		new EditorLoadingScreenEntry("rochie-1727883649.edds", "Rochie"),
		new EditorLoadingScreenEntry("typiq-1727399203.edds", "Typiq")
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
		
		m_ImageWidgetBackground.LoadImageFile(0, string.Format("DayZEditor/gui/loadingscreens/%1", LOADING_SCREENS[val].FileName));
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