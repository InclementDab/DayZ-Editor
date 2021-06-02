modded class LoadingScreen
{
	static const int LOADING_SCREEN_COUNT = 13;
	
	void LoadingScreen(DayZGame game)
	{		
		m_ImageLogoMid.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		m_ImageLogoMid.SetImage(0);
		m_ImageLogoMid.SetFlags(WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		
		m_ImageLogoCorner.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		m_ImageLogoCorner.SetImage(0);
		m_ImageLogoCorner.SetFlags(WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND);
		
		m_ImageLogoMid.Show(true);
		m_ImageLogoMid.SetSize(480, 270);
		
		float x, y, w, h;
		m_ImageLogoMid.GetPos(x, y);
		x -= 60; // i just dont like the original position
		m_ImageLogoMid.SetPos(x, y);
		m_ImageLogoMid.GetSize(w, h);
		m_ImageLogoMid.SetColor(ARGB(150, 30, 30, 30));
		m_ImageLogoCorner.Show(true);
		m_ImageLogoCorner.SetPos(x - 3, y - 3);
		m_ImageLogoCorner.SetSize(w, h);
		m_ImageLogoCorner.SetColor(ARGB(220, 255, 255, 255));
		
		m_ImageWidgetBackground.SetFlags(WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND);
		//m_EditorLogo = game.GetLoadingWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorLogo.layout", m_ImageWidgetBackground);
		
		int s_x, s_y;
		GetScreenSize(s_x, s_y);
		m_ProgressLoading.SetSize(s_x, 8);
				
		m_ModdedWarning.SetText("WARNING! The DayZ Editor is currently in BETA. Please report all bugs you find to our Discord");	
		m_ModdedWarning.Show(false);
		
		// nice
		int val = TickCount(0);
		val = val + (val << 7);
		val = val + (val << 19);
		val = val + (val << 37);
		val = Math.AbsInt(val);
		val = val % LOADING_SCREEN_COUNT + 1;
		m_ImageWidgetBackground.LoadImageFile(0, string.Format("DayZEditor/gui/loadingscreens/%1.edds", val));
		m_ImageWidgetBackground.SetImage(0);
	}
}