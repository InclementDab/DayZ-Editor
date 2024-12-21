class EditorMainMenuController: ViewController
{
	ref ObservableCollection<ref ScriptView> MapViews = new ObservableCollection<ref ScriptView>(this);
	ref ObservableCollection<ref ScriptView> StatisticsEntries = new ObservableCollection<ref ScriptView>(this);
}

class EditorStatisticsEntryView: ScriptView
{
	TextWidget Text, Value;
	
	void EditorStatisticsEntryView(string text, string value)
	{
		Text.SetText(text);
		Value.SetText(value);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\items\\EditorStatisticsEntry.layout";
	}
}

class Payload_EditorLogin: Managed
{
	string SteamId;
	string DzGuid;
	int PlayTime;	
	int ItemsPlaced;
	int ItemsDeleted;
	int CamerasPlaced;
	int CamerasRode;
	float DistanceFlown;
	int CharactersControlled;
	int CharactersEdited;
}

class Payload_ServerShowcase: Managed
{
	string name;
	string imageUrl;
	string redirectUrl;
	string identifier;
}

class Payload_ServerShowcaseReport: Managed
{
	string Identifier;
	string Reporter;
}

class Payload_EditorLoginResponse: Managed
{
	string Message;
	int CurrentLoginCount;
	string Token;
	ref array<ref Payload_ServerShowcase> Showcases = {};
}

class EditorLoginCallback: RestCallbackBase
{
	protected ref ScriptCaller m_OnPayloadSuccess;

	void EditorLoginCallback(notnull ScriptCaller on_payload_success)
	{
		m_OnPayloadSuccess = on_payload_success;
	}

    override void OnSuccess(string data, int dataSize)
	{
		super.OnSuccess(data, dataSize);
		
		Payload_EditorLoginResponse response = new Payload_EditorLoginResponse();
		string error;
		if (!JsonFileLoader<Payload_EditorLoginResponse>.LoadData(data, response, error)) {
			Error(error);
			return;
		}
		
		m_OnPayloadSuccess.Invoke(response);
	}
}

class EditorMainMenu: ScriptViewMenu
{
	protected EditorMainMenuController m_TemplateController;
	protected float m_MotionSicknessDt, m_ShowcaseTime;
	protected ref Payload_EditorLoginResponse m_PayloadLoginInfoCache;
	protected int m_ShowcaseIndex, m_IsShowcaseActive;
	protected ref array<int> m_ShowcasesReported = {};

	Widget ServerShowcase, ServerShowcaseOutline, MapGrid, MapSelectorFrame;
	ImageWidget MapSelectorBackground, ServerShowcaseImage;
	ButtonWidget ExitButton, SettingButton, DiscordButton, WikiButton, TwitterButton, PrevServerShowcase, NextServerShowcase;
	TextWidget VersionText, EditorText, StatHeaderText;
	RichTextWidget ServerShowcaseBackupText;
	ScrollWidget MapScroller;
	
	void EditorMainMenu()
	{
		// Slow rollout
		m_IsShowcaseActive = (Math.RandomInt(0, 4) == 0);
		
#ifdef DIAG_DEVELOPER
		m_IsShowcaseActive = 1; // fast rollout
#endif
		
		m_TemplateController = EditorMainMenuController.Cast(m_Controller);
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (GetGame().ConfigIsExisting(string.Format("CfgWorlds %1 worldName", name))) {
				if (m_TemplateController.MapViews.Count() != 0) {
					QuickView<Widget> Spacer = new QuickView<Widget>();
					Spacer.Root.SetScreenSize(24, 24);
					m_TemplateController.MapViews.Insert(Spacer);
				}

				m_TemplateController.MapViews.Insert(new EditorMapView(name));
			}
		}
		
		float mg_s_w, mg_s_h;
		MapGrid.GetScreenSize(mg_s_w, mg_s_h);
		//MapSelectorBackground.SetScreenSize(mg_s_h * m_TemplateController.MapViews.Count() / 2, mg_s_h);
		
		string version;
		GetGame().GetVersion(version);
		VersionText.SetText(string.Format("#main_menu_version %1", version));
		EditorText.SetText(string.Format("#STR_EDITOR_MAIN_MENU_VERSION %1, created by InclementDab", Editor.Version));
		
		// Update global login counter
		EditorStatistics statistics = EditorStatistics.Cast(GetDayZGame().GetProfileSetting(EditorStatistics));
		string uid = GetGame().GetUserManager().GetSelectedUser().GetUid();
		Payload_EditorLogin login_payload = new Payload_EditorLogin();
		login_payload.SteamId = uid;
		login_payload.PlayTime = statistics.EditorPlayTime;
		login_payload.ItemsPlaced = statistics.EditorPlacedObjects;
		login_payload.ItemsDeleted = statistics.EditorRemovedObjects;
		login_payload.CamerasPlaced = statistics.EditorPlacedCameraTracks;
		login_payload.CamerasRode = statistics.EditorCameraTracksRidden;
		login_payload.DistanceFlown = statistics.DistanceFlown;		
		login_payload.CharactersControlled = statistics.CharactersControlled;
		login_payload.CharactersEdited = statistics.CharactersEdited;
		
		string payload, error;
		if (JsonFileLoader<Payload_EditorLogin>.MakeData(login_payload, payload, error, false)) {
			RestContext ctx = CreateRestApi().GetRestContext(Editor.WEB_API_ENDPOINT);
			ctx.SetHeader("application/json\r\nUser-Agent: DayZ-Editor");
			ctx.POST(new EditorLoginCallback(ScriptCaller.Create(OnLoginResponse)),"api\/user\/login", payload);
		}
		
		StatHeaderText.SetText(string.Format("Welcome, %1", GetGame().GetUserManager().GetTitleInitiator().GetName()));
		ServerShowcaseBackupText.SetText("Want your service here?\nUse '/showcase request' in Discord\nClick to join.");
	}

	protected void OnLoginResponse(Payload_EditorLoginResponse response)
	{
		m_PayloadLoginInfoCache = response;
		
		// Get statistics
		EditorStatistics statistics = EditorStatistics.Cast(GetDayZGame().GetProfileSetting(EditorStatistics));
		TimeSpan time = statistics.EditorPlayTime;
		
		string placed = statistics.EditorPlacedObjects.ToString();
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Placed Objects", placed));
		
		string removed = statistics.EditorRemovedObjects.ToString();
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Removed Objects", removed));
		
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Distance Travelled", string.Format("%1km", statistics.DistanceFlown / 1000)));
		string controlled = statistics.CharactersControlled.ToString();
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Characters Controlled", controlled));
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Time Spent Editing", time.Format()));
		
		string cache_folder = SystemPath.Saves("EditorCache");
		MakeDirectory(cache_folder);
		string img_folder = SystemPath.Combine(cache_folder, "img");
		MakeDirectory(img_folder);
		if (m_IsShowcaseActive) {
			for (int i = 0; i < response.Showcases.Count(); i++) {
				Payload_ServerShowcase showcase = response.Showcases[i];
				string file_name = string.Format("%1.dds", showcase.name);
				array<string> url_split = {};
				showcase.imageUrl.Split("?", url_split);
				RestContext image_ctx = GetRestApi().GetRestContext(url_split[0]);
				image_ctx.SetHeader("application/octet-stream");
				image_ctx.FILE(new RestCallbackBase(), "?" + url_split[1], file_name);
				
				string dst_file = SystemPath.Combine(img_folder, file_name);
				string src_file = SystemPath.Profile(string.Format("Users/Survivor/%1", file_name));
				if (!FileExist(src_file)) {
					src_file = SystemPath.Saves(file_name);
				}
	
				if (FileExist(src_file)) {
					CopyFile(src_file, dst_file);
					DeleteFile(src_file);
				}
				
				ServerShowcaseImage.LoadImageFile(i, dst_file);			
			}
		}

		m_ShowcaseIndex = 0;
		ServerShowcaseImage.SetImage(0);
		ServerShowcaseImage.Show(m_IsShowcaseActive);
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
				
		if (!GetGame().IsAppActive()) {
			return;
		}
		
		float mg_s_w, mg_s_h;
		MapGrid.GetScreenSize(mg_s_w, mg_s_h);
		
		float mss_s_w, mss_s_h;
		MapSelectorFrame.GetScreenSize(mss_s_w, mss_s_h);
		if (mg_s_w > mss_s_w) {
			// by default, this color will blend into the background to remain invisible
			MapScroller.SetColor(-1);
		}
		
		int mouse_x, mouse_y, screen_x, screen_y;
	    GetMousePos(mouse_x, mouse_y);
	    GetScreenSize(screen_x, screen_y);
		
		if (m_IsShowcaseActive && m_PayloadLoginInfoCache && GetWidgetUnderCursor() != ServerShowcase) {
			m_ShowcaseTime += dt;
			auto showcase = m_PayloadLoginInfoCache.Showcases[m_ShowcaseIndex];
			if (m_ShowcaseTime > 2.0 && m_ShowcasesReported.Find(m_ShowcaseIndex) == -1) {
				string payload, error;
				Payload_ServerShowcaseReport report_payload = new Payload_ServerShowcaseReport();
				report_payload.Identifier = showcase.identifier;
				report_payload.Reporter = GetGame().GetUserManager().GetTitleInitiator().GetUid();
				if (JsonFileLoader<Payload_ServerShowcaseReport>.MakeData(report_payload, payload, error)) {
					RestContext ctx = GetRestApi().GetRestContext(Editor.WEB_API_ENDPOINT);
					ctx.SetHeader("application/json\r\nUser-Agent: DayZ-Editor");
					ctx.POST(new RestCallbackBase(), "api\/showcase\/report-impression", payload);
				}
				
				m_ShowcasesReported.Insert(m_ShowcaseIndex);
			}
			
			if (m_ShowcaseTime > 10.0) {
				m_ShowcaseIndex = Math.Rollover(m_ShowcaseIndex + 1, 0, m_PayloadLoginInfoCache.Showcases.Count());
				ServerShowcaseImage.SetImage(m_ShowcaseIndex);
				m_ShowcaseTime = 0;
			}
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		Widget child_icon = w.FindAnyWidget(string.Format("%1_Icon", w.GetName()));
		ImageWidget child_image = ImageWidget.Cast(child_icon);
		if (child_image && w.IsInherited(ButtonWidget)) {
			WidgetAnimator.Animate(child_image, WidgetAnimatorProperty.SIZE_H, 1.0, 90);
			WidgetAnimator.Animate(child_image, WidgetAnimatorProperty.SIZE_W, 1.0, 90);
		}
			
		switch (w) {			
			case ExitButton: {
				child_image.SetColor(LinearColor.INDIAN_RED);
				child_image.SetImage(3);
				GetDayZGame().CreateDelayedTooltip(w, "#main_menu_exit", TooltipPosition.INSIDE);
				break;
			}

			case SettingButton: {
				child_image.SetColor(LinearColor.LIGHT_BLUE);
				child_image.SetImage(3);
				GetDayZGame().CreateDelayedTooltip(w, "#main_menu_options", TooltipPosition.INSIDE);
				break;
			}

			case DiscordButton: {
				child_image.SetColor(LinearColor.DISCORD);
				GetDayZGame().CreateDelayedTooltip(w, "https:\/\/discord.gg\/dayz-editor", TooltipPosition.INSIDE);
				break;
			}

			case WikiButton: {
				child_image.SetColor(LinearColor.GOLDENROD);
				child_image.SetImage(3);
				GetDayZGame().CreateDelayedTooltip(w, "https:\/\/github.com\/InclementDab\/DayZ-Editor", TooltipPosition.INSIDE);
				break;
			}			
			
			case TwitterButton: {
				child_image.SetColor(LinearColor.TWITTER);
				GetDayZGame().CreateDelayedTooltip(w, "https:\/\/twitter.com\/InclementDab", TooltipPosition.INSIDE);
				break;
			}
			
			case NextServerShowcase: {
				child_image.SetImage(3);
				GetDayZGame().CreateDelayedTooltip(w, "Next", TooltipPosition.INSIDE);
				break;
			}
			
			case PrevServerShowcase: {
				child_image.SetImage(3);
				GetDayZGame().CreateDelayedTooltip(w, "Previous", TooltipPosition.INSIDE);
				break;
			}

			case ServerShowcase: {
				ServerShowcaseOutline.SetColor(EditorColors.BLUE);
				if (m_IsShowcaseActive && m_PayloadLoginInfoCache) {
					auto showcase = m_PayloadLoginInfoCache.Showcases[m_ShowcaseIndex];
					GetDayZGame().CreateDelayedTooltip(w, showcase.name, TooltipPosition.TOP_RIGHT);
				} else {
					GetDayZGame().CreateDelayedTooltip(w, "https:\/\/discord.gg\/dayz-editor", TooltipPosition.INSIDE);
				}

				break;
			}
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		GetDayZGame().ClearTooltip();
		
		Widget child_icon = w.FindAnyWidget(string.Format("%1_Icon", w.GetName()));
		ImageWidget child_image = ImageWidget.Cast(child_icon);		
		switch (w) {
			case ExitButton:
			case SettingButton: {
				child_image.SetImage(2);
				break;
			}

			case ServerShowcase: {
				WidgetAnimator.AnimateColor(ServerShowcaseOutline, -1, 60);
				return true;
			}
			
			case NextServerShowcase: {
				child_image.SetImage(2);
				break;
			}
			
			case PrevServerShowcase: {
				child_image.SetImage(2);
				break;
			}
		}

		if (child_image && w.IsInherited(ButtonWidget)) {
			WidgetAnimator.CancelAnimate(child_image);
			child_image.SetSize(0.8, 0.8);
			WidgetAnimator.AnimateColor(child_image, -1, 100);
		}
				
		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return super.OnMouseButtonUp(w, x, y, button);	
		}

		switch (w) {
			case ServerShowcase: {
				if (m_IsShowcaseActive && m_PayloadLoginInfoCache) {
					auto showcase = m_PayloadLoginInfoCache.Showcases[m_ShowcaseIndex];
					if (showcase) {
						GetGame().OpenURL(showcase.redirectUrl);
					}
				} else {
					GetGame().OpenURL("https:\/\/discord.gg\/dayz-editor");
				}

				break;
			}
		}

		return super.OnMouseButtonUp(w, x, y, button);
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
			
			case NextServerShowcase: {
				m_ShowcaseIndex = Math.Rollover(m_ShowcaseIndex + 1, 0, m_PayloadLoginInfoCache.Showcases.Count());
				ServerShowcaseImage.SetImage(m_ShowcaseIndex);
				m_ShowcaseTime = 0;
				break;
			}
			
			case PrevServerShowcase: {
				m_ShowcaseIndex = Math.Rollover(m_ShowcaseIndex - 1, 0, m_PayloadLoginInfoCache.Showcases.Count());
				ServerShowcaseImage.SetImage(m_ShowcaseIndex);
				m_ShowcaseTime = 0;
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