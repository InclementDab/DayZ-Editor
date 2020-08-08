modded class LoadingScreen
{
	void LoadingScreen(DayZGame game)
	{
		m_ImageLogoMid.LoadImageFile(0, "DayZEditor/gui/images/dayz_editor_logo.edds");
		m_ImageLogoMid.SetImage(0);
		m_ImageLogoMid.SetFlags(m_ImageLogoMid.GetFlags() | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		m_ImageLogoCorner.LoadImageFile(0, "DayZEditor/gui/images/dayz_editor_logo.edds");
		m_ImageLogoCorner.SetImage(0);
		m_ImageLogoCorner.SetFlags(m_ImageLogoCorner.GetFlags() | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
	}
}