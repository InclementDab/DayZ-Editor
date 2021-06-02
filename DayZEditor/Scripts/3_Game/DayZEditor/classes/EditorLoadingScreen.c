modded class LoadingScreen
{
	static const int LOADING_SCREEN_COUNT = 12;
	
	protected bool m_EditorImagePicked;
	
	void LoadingScreen(DayZGame game)
	{		
		m_ImageLogoMid.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		m_ImageLogoMid.SetImage(0);
		m_ImageLogoMid.SetFlags(WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);

		
		m_ImageLogoCorner.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		m_ImageLogoCorner.SetImage(0);
		m_ImageLogoCorner.SetFlags(WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND);
		
		m_ImageLogoMid.Show(true);
		m_ImageLogoCorner.Show(true);
		
		m_ImageWidgetBackground.SetFlags(WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND);
		//m_EditorLogo = game.GetLoadingWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorLogo.layout", m_ImageWidgetBackground);
				
		m_ModdedWarning.SetText("WARNING! The DayZ Editor is currently in BETA. Please report all bugs you find to our Discord");	
		m_ModdedWarning.Show(true);
	}
	
	
	override void Show()
	{
		super.Show();
		
		// this solution is hacky, but Math.Random doesnt work in the constructor of this
		if (!m_EditorImagePicked) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(_LoadRandomImage, 25);
		}
	}
	
	private void _LoadRandomImage()
	{
		m_ImageWidgetBackground.LoadImageFile(0, string.Format("DayZEditor/gui/loadingscreens/%1.edds", Math.RandomIntInclusive(0, LOADING_SCREEN_COUNT)));
		m_ImageWidgetBackground.SetImage(0);
		
		// e a s t e r e g g
		if (Math.RandomInt(0, 100) == 69)
			m_ModdedWarning.SetText("You are running a modded version of the game. If shits fucked? Deal with it.");
		
		m_EditorImagePicked = true;
	}
}