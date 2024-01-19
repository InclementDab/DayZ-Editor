// this is only used once and thats on engine boot. otherwise its our new scripted menu
// layout root will need to be ref'd here since the engine is starting. the UI Manager isnt reffing any Widgets

// this class is special i swear to god
typedef Param2<string, int> EditorLoadingScreenEntry;

modded class LoadingScreen
{	
	static ref array<string> IMAGES = {
		"Editor/gui/images/prarie.edds"
	};
	
	protected DayZGame m_Game;
	protected ProgressBarWidget m_LoadingBar;
	protected ImageWidget m_LoadingImage, m_SandboxImage;
		
	void LoadingScreen(DayZGame game)
	{
		m_Game = game;
		
		delete m_HintPanel;
		if (m_WidgetRoot) {
			m_WidgetRoot.Unlink();
			delete m_WidgetRoot;
		}
		
		m_WidgetRoot = m_Game.GetLoadingWorkspace().CreateWidgets("Editor\\GUI\\layouts\\Loading.layout");
		m_LoadingBar = ProgressBarWidget.Cast(m_WidgetRoot.FindAnyWidget("ProgressBar"));
		m_LoadingImage = ImageWidget.Cast(m_WidgetRoot.FindAnyWidget("LoadingImage"));
		m_SandboxImage = ImageWidget.Cast(m_WidgetRoot.FindAnyWidget("SandboxImage"));
		
		Math.Randomize(-1);
		m_LoadingImage.LoadImageFile(0, IMAGES.GetRandomElement());
		m_LoadingImage.SetImage(0);
		
		// engine calls this when loading
		ProgressAsync.SetProgressData(m_LoadingBar);
		ProgressAsync.SetUserData(m_SandboxImage);
	}
	
	void ~LoadingScreen()
	{
		if (m_WidgetRoot) {
			m_WidgetRoot.Unlink();
			delete m_WidgetRoot;
		}
		
		// cleaning up after loading
		ProgressAsync.SetProgressData(null);
		ProgressAsync.SetUserData(null);
	}
	
	override void Show()
	{
		m_WidgetRoot.Show(true);
	}
	
	override void Hide(bool force)
	{
		m_WidgetRoot.Show(false);
	}
	
	override void SetTitle(string title)
	{
		//m_TextWidgetTitle.SetText(title);
	}
	
	override void SetStatus(string status)
	{
		//m_TextWidgetStatus.SetText(status);
	}
	
	override void SetProgress(float val)
	{
		float time_delta = m_DayZGame.GetTickTime() - m_LastProgressUpdate;
		
		m_LastProgressUpdate = m_DayZGame.GetTickTime();
	}
}