class EditorMapView: ScriptView
{
	protected float m_MotionSicknessDt;
	protected string m_MapConfig, m_ImageNormal, m_ImageHover;
	protected AbstractWave m_CurrentMapSound;
	ImageWidget Image;
	TextWidget Text;
	Widget Outline;

	void EditorMapView(string map_config)
	{
		m_MapConfig = map_config;
		m_ImageNormal = GetGame().ConfigGetTextOut(string.Format("CfgWorlds %1 mapTextureClosed", m_MapConfig));
		m_ImageHover = GetGame().ConfigGetTextOut(string.Format("CfgWorlds %1 mapTextureOpened", m_MapConfig));
		string map_name = GetGame().ConfigGetTextOut(string.Format("CfgWorlds %1 description", m_MapConfig));
		Text.SetText(map_name);
		Image.LoadImageFile(0, m_ImageNormal);
		Image.LoadImageFile(1, m_ImageHover);
		Image.SetImage(0);
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		Widget widget_under_cursor = GetWidgetUnderCursor();
		if (widget_under_cursor != m_LayoutRoot) {
			return;
		}
		
		int mouse_x, mouse_y, screen_x, screen_y;
	    GetMousePos(mouse_x, mouse_y);
	    GetScreenSize(screen_x, screen_y);
		
		m_MotionSicknessDt += dt;
		if (m_MotionSicknessDt > (1 / 30)) {			
			const float PARALLAX_AMOUNT = 0.5;
	        float x_relative = ((mouse_x / screen_x) * PARALLAX_AMOUNT) - (PARALLAX_AMOUNT / 2); // 5.0 is from my extra percent size / 2
	        float y_relative = ((mouse_y / screen_y) * PARALLAX_AMOUNT) - (PARALLAX_AMOUNT / 2);
			
	        Image.SetPos(x_relative / 100, y_relative / 100);
			m_MotionSicknessDt = 0;
		}
		
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		Image.SetImage(1);

		EffectSound snd;
		Camera.GetCurrentCamera().PlaySoundSet(snd, "mapIn_SoundSet", 0, 0);
		
		return true;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Image.SetImage(0);

		EffectSound snd;
		Camera.GetCurrentCamera().PlaySoundSet(snd, "mapOut_SoundSet", 0, 0);
		return true;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorMapView.layout";
	}
}

class EditorMainMenuController: ViewController
{
	ref ObservableCollection<ref EditorMapView> MapViews = new ObservableCollection<ref EditorMapView>(this);
}

class EditorMainMenu: ScriptViewMenu
{
	protected EditorMainMenuController m_TemplateController;

	void EditorMainMenu()
	{
		m_TemplateController = EditorMainMenuController.Cast(m_Controller);
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (GetGame().ConfigIsExisting(string.Format("CfgWorlds %1 worldName", name))) {
				m_TemplateController.MapViews.Insert(new EditorMapView(name));
			}
		}
	}

	override void Update(float dt)
	{
		super.Update(dt);
	}

	override bool UseMouse()
	{
		return true;
	}

	override typename GetControllerType()
	{
		return EditorMainMenuController;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorMainMenu2.layout";
	}
}