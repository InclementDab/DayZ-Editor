modded class LoadingScreen
{
	private Timer m_Timer;
	void LoadingScreen(DayZGame game)
	{
		m_ImageLogoMid.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		m_ImageLogoMid.SetImage(0);
		m_ImageLogoMid.SetFlags(m_ImageLogoMid.GetFlags() | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		m_ImageLogoCorner.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		m_ImageLogoCorner.SetImage(0);
		m_ImageLogoCorner.SetFlags(m_ImageLogoCorner.GetFlags() | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		
		m_ImageLogoMid.Show(false);
		m_ImageLogoCorner.Show(false);
		
		m_ImageWidgetBackground.LoadImageFile(0, "DayZEditor/gui/images/dayz_editor_splash0.edds");
		m_ImageWidgetBackground.SetImage(0);
		m_ImageWidgetBackground.SetFlags(m_ImageWidgetBackground.GetFlags() | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		m_ImageWidgetBackground.GetPos(back_x, back_y);
		
		m_ModdedWarning.SetText("WARNING! You are running a modded version of the game. If shits fucked? Deal with it.");
		
		float x, y;
		m_ImageLogoMid.GetPos(x, y);
		m_ModdedWarning.SetPos(x, y - 40);
		
		Widget bottom = m_WidgetRoot.FindAnyWidget("BottomPanel");
		int s_x, s_y;
		float b_x, b_y;
		GetScreenSize(s_x, s_y);

		bottom.GetSize(b_x, b_y);
		m_ProgressLoading.SetSize(s_x, 6);
		m_ProgressLoading.SetPos(0, b_y * 2);
		m_ProgressLoading.Update();
	}
	
	override void Show()
	{
		m_ImageLogoMid.Show(false);
		m_ImageLogoCorner.Show(false);
	}

	float back_x, back_y;

	// todo
	void FindAnUpdateLoopSoYOuCanRunThis(float val)
	{
		super.SetProgress(val);
		
		float time_delta = m_DayZGame.GetTickTime() - m_LastProgressUpdate;
		
		float duration = 10;
		time_delta /= 1000;
		
		float normalized_time = (1 / duration) * time_delta;
		normalized_time = Math.Clamp(normalized_time, 0, 1);
		// m_ImageWidgetBackground.GetMaskProgress() instead of this for 0...1
		float pos_x = Math.Lerp(back_x, back_x + 50, normalized_time);
		float pos_y = Math.Lerp(back_y, back_y + 50, normalized_time);
		Print(pos_x);
		Print(pos_y);
		m_ImageWidgetBackground.SetPos(pos_x, pos_y);
		m_ImageWidgetBackground.Update();
	}
}