class EditorMainMenuController: ViewController
{
	ref ObservableCollection<ref EditorMapView> MapViews = new ObservableCollection<ref EditorMapView>(this);
}

class EditorMainMenu: ScriptViewMenu
{
	protected EditorMainMenuController m_TemplateController;
	protected float m_MotionSicknessDt;

	ImageWidget MapSelectorBackground;
	ButtonWidget ExitButton, SettingButton, DiscordButton, WikiButton, TwitterButton;
	TextWidget VersionText, EditorText;
	
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
		
		string version;
		GetGame().GetVersion(version);
		VersionText.SetText(string.Format("#main_menu_version %1", version));
		
		EditorText.SetText(string.Format("#STR_EDITOR_MAIN_MENU_VERSION %1, created by InclementDab", Editor.Version));
		
		// Update global login counter
		string uid = GetGame().GetUserManager().GetSelectedUser().GetUid();
		RestContext ctx = CreateRestApi().GetRestContext(Editor.WEB_API_ENDPOINT);
		ctx.SetHeader("application/json\r\nUser-Agent: DayZ-Editor");
		ctx.POST(new RestCallbackBase(),"api\/update-login-counter", string.Format("{\"id\":%1}", uid));
	}

	override void Update(float dt)
	{
		super.Update(dt);
		
		int mouse_x, mouse_y, screen_x, screen_y;
	    GetMousePos(mouse_x, mouse_y);
	    GetScreenSize(screen_x, screen_y);
		
		m_MotionSicknessDt += dt;
		if (m_MotionSicknessDt > (1 / 30)) {			
			const float PARALLAX_AMOUNT = 0.5;
	        float x_relative = ((mouse_x / screen_x) * PARALLAX_AMOUNT) - (PARALLAX_AMOUNT / 2); // 5.0 is from my extra percent size / 2
	        float y_relative = ((mouse_y / screen_y) * PARALLAX_AMOUNT) - (PARALLAX_AMOUNT / 2);
	        MapSelectorBackground.SetPos(x_relative * 10, y_relative * 10);
			m_MotionSicknessDt = 0;
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		ImageWidget child_image = ImageWidget.Cast(w.GetChildren());
		if (child_image) {
			WidgetAnimator.Animate(child_image, WidgetAnimatorProperty.SIZE_H, 1.0, 90);
			WidgetAnimator.Animate(child_image, WidgetAnimatorProperty.SIZE_W, 1.0, 90);
		}
		
		switch (w) {
			case ExitButton: {
				child_image.SetColor(LinearColor.INDIAN_RED);
				child_image.SetImage(3);
				break;
			}

			case SettingButton: {
				child_image.SetColor(LinearColor.LIGHT_BLUE);
				child_image.SetImage(3);
				break;
			}

			case DiscordButton: {
				child_image.SetColor(LinearColor.DISCORD);
				break;
			}

			case WikiButton: {
				child_image.SetColor(LinearColor.GOLDENROD);
				child_image.SetImage(3);
				break;
			}			
			
			case TwitterButton: {
				child_image.SetColor(LinearColor.TWITTER);
				break;
			}
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		ImageWidget child_image = ImageWidget.Cast(w.GetChildren());
		switch (w) {
			case ExitButton:
			case WikiButton:
			case SettingButton: {
				child_image.SetImage(2);
				break;
			}
		}

		if (child_image) {
			child_image.SetSize(0.8, 0.8);
			WidgetAnimator.AnimateColor(child_image, -1, 100);
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return super.OnClick(w, x, y, button);
		}
		
		switch (w) {
			case ExitButton: {
				ShowDialog("#main_menu_exit", "#main_menu_exit_desc", IDC_MAIN_QUIT, DBT_YESNO, DBB_YES, DMT_QUESTION);
				break;
			}

			case SettingButton: {
				EnterChildMenu(MENU_OPTIONS);
				break;
			}

			case DiscordButton: {
				GetGame().OpenURL("https:\/\/discord.gg\/dayz-editor");
				break;
			}

			case WikiButton: {
				GetGame().OpenURL("https:\/\/github.com\/InclementDab\/DayZ-Editor");
				break;
			}

			case TwitterButton: {
				GetGame().OpenURL("https:\/\/twitter.com\/InclementDab");
				break;
			}
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		switch (code) {
			case IDC_MAIN_QUIT: {
				if (result == 2) {
					GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(g_Game.RequestExit, IDC_MAIN_QUIT);
				}

				break;
			}
		}

		return super.OnModalResult(w, x, y, code, result);
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