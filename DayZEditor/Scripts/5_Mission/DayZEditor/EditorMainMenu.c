class EditorMainMenuController : ViewController
{
	ref ObservableCollection<ref ScriptView> MapViews = new ObservableCollection<ref ScriptView>(this);
	ref ObservableCollection<ref ScriptView> StatisticsEntries = new ObservableCollection<ref ScriptView>(this);
}

class EditorStatisticsEntryView : ScriptView
{
	TextWidget Text, Value;

	void EditorStatisticsEntryView(string text, string value, bool global)
	{
		Text.SetText(text);
		Value.SetText(string.Format("%1%2", value, Ternary<string>.If(global, " (Global)", string.Empty)));
	}

	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\items\\EditorStatisticsEntry.layout";
	}
}

class SimpleButton: ScriptWidgetBase
{
	ButtonWidget Button;

	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);

		Button = ButtonWidget.Cast(w);
		if (!Button) {
			Error("SimpleButton script must be defined on Button Widget!");
			return;
		}
	}

	/*
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		

		return true;
	}

	override bool OnMouseLeave(Widget w, int x, int y, Widget enterW)
	{
		return true;
	}*/
}

class EditorMainMenu: ScriptViewMenu
{
	protected EditorMainMenuController m_TemplateController;
	protected float m_MotionSicknessDt, m_ShowcaseTime;
	protected int m_ShowcaseIndex, m_IsShowcaseActive;
	protected ref array<int> m_ShowcasesReported = { };
	protected ref array<int> m_ValidShowcaseSlots = {}; // extra check in case an image fails to load

	protected float m_SoundVolume = 1.0, m_MusicVolume = 1.0;
	protected bool m_GlobalStatsVisible;
	
	Widget ServerShowcase, ServerShowcaseOutline, MapGrid, MapSelectorFrame, GlobeFrame;
	ImageWidget MapSelectorBackground, ServerShowcaseImage;
	ButtonWidget ExitButton, SettingButton, DiscordButton, WikiButton, TwitterButton, PrevServerShowcase, NextServerShowcase;
	TextWidget VersionText, EditorText, StatHeaderText;
	RichTextWidget ServerShowcaseBackupText;
	ScrollWidget MapScroller;
	ImageWidget SoundButton, MusicButton, GlobeButton;

	void EditorMainMenu()
	{
		// Slow rollout
		m_IsShowcaseActive = (Math.RandomInt(0, 2) == 0);

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

		StatHeaderText.SetText(string.Format("Welcome, %1", GetGame().GetUserManager().GetTitleInitiator().GetName()));
		ServerShowcaseBackupText.SetText("Want your service here?\nUse '/showcase_request' in Discord\nClick to join.");
		
		m_SoundVolume = GetGame().GetSoundScene().GetSoundVolume();
		m_MusicVolume = GetGame().GetSoundScene().GetMusicVolume();
		
		if (m_SoundVolume <= 0.0) {
			Symbols.VOLUME_SLASH.Load(SoundButton);
		}
		
		if (m_MusicVolume <= 0.0) {
			Symbols.MUSIC_SLASH.Load(MusicButton);
		}
		
		// Update global login counter
		EditorStatistics statistics = EditorStatistics.Cast(GetDayZGame().GetProfileSetting(EditorStatistics));
		string uid = GetDayZGame().GetUserManager().GetSelectedUser().GetUid();
		string username = GetDayZGame().GetUserManager().GetSelectedUser().GetName();
		Payload_EditorLogin login_payload = new Payload_EditorLogin();
		login_payload.SteamId = uid;
		login_payload.SteamUsername = username;
		login_payload.PlayTime = statistics.EditorPlayTime;
		login_payload.ItemsPlaced = statistics.EditorPlacedObjects;
		login_payload.ItemsDeleted = statistics.EditorRemovedObjects;
		login_payload.CamerasPlaced = statistics.EditorPlacedCameraTracks;
		login_payload.CamerasRode = statistics.EditorCameraTracksRidden;
		login_payload.DistanceFlown = statistics.DistanceFlown;
		login_payload.CharactersControlled = statistics.CharactersControlled;
		login_payload.CharactersEdited = statistics.CharactersEdited;

		string payload, error;
		if (GetDayZGame().LoginCache) {
			OnLoginResponse(GetDayZGame().LoginCache);
		}
		else if (JsonFileLoader<Payload_EditorLogin>.MakeData(login_payload, payload, error, false)) {
			RestContext ctx = CreateRestApi().GetRestContext(Editor.WEB_API_ENDPOINT);
			ctx.SetHeader("application/json\r\nUser-Agent: DayZ-Editor");
			ctx.POST(new EditorLoginCallback(ScriptCaller.Create(OnLoginResponse)), "api\/user\/login", payload);
		}
		
		if (!Editor.Experimental) {
			RestContext version_ctx = CreateRestApi().GetRestContext(Editor.WEB_API_ENDPOINT);
			version_ctx.GET(new EditorVersionCallback(ScriptCaller.Create(OnVersionResponse)), "api\/changelog\/Version");
		}
	}

	protected void OnVersionResponse(Payload_EditorVersionResponse version_response)
	{		
		// We never started major / minor versions in the db so i'm just doing it like this
		if (version_response.Version != Editor.Version) {
			ShowDialog("Editor Version Mismatch", string.Format("Please repair DayZ Editor in the Launcher to get the latest tool version.\n\nMODS -> Right click 'DayZ Editor' -> Repair\n\n(%1 < %2)", version_response.Version, Editor.Version), 1432, DBT_OK, DBB_OK, DMT_EXCLAMATION);
		}
	}

	protected void OnLoginResponse(Payload_EditorLoginResponse login_cache)	
	{
		GetDayZGame().LoginCache = login_cache;

		string cache_folder = SystemPath.Saves("EditorCache");
		MakeDirectory(cache_folder);
		string img_folder = SystemPath.Combine(cache_folder, "img");
		MakeDirectory(img_folder);
		if (m_IsShowcaseActive && login_cache.Showcases.Count()) {
			// Reesize
			m_ValidShowcaseSlots.Resize(login_cache.Showcases.Count());
			
			for (int j = 0; j < login_cache.Showcases.Count(); j++) {
				Payload_ServerShowcase showcase = login_cache.Showcases[j];
				string file_name = string.Format("%1.dds", showcase.Name);
				RestContext image_ctx = GetRestApi().GetRestContext(showcase.ImageUrl);
				image_ctx.SetHeader("application/octet-stream");
				image_ctx.FILE(new RestCallbackBase(), "", file_name);

				string dst_file = SystemPath.Combine(img_folder, file_name);
				string src_file = SystemPath.Profile(string.Format("Users/Survivor/%1", file_name));
				
				if (!FileExist(src_file)) {
					src_file = SystemPath.Saves(file_name);
				}
				
				if (FileExist(src_file)) {
					if (CopyFile(src_file, dst_file)) {
						DeleteFile(src_file);
					}
				}

				m_ValidShowcaseSlots[j] = ServerShowcaseImage.LoadImageFile(j, dst_file);
			}

			m_ShowcaseIndex = 0;
			
			if (m_ValidShowcaseSlots[m_ShowcaseIndex]) {
				ServerShowcaseImage.SetImage(m_ShowcaseIndex);
				ServerShowcaseImage.Show(true);
			}
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetStatisticsMode, 0, 0, m_GlobalStatsVisible);
	}
	
	protected void SetStatisticsMode(bool global)
	{
		if (!GetDayZGame().LoginCache) {
			global = false;
			GlobeFrame.Show(false);
		} else {
			GlobeFrame.Show(true);
		}
		
		m_GlobalStatsVisible = global;
		
		if (!m_GlobalStatsVisible) {
			Symbols.GLOBE.Load(GlobeButton);
		} else {
			Symbols.CHART_SIMPLE.Load(GlobeButton);
		}
		
		// Get statistics
		EditorStatistics statistics = EditorStatistics.Cast(GetDayZGame().GetProfileSetting(EditorStatistics));
		TimeSpan time = statistics.EditorPlayTime;

		string placed = statistics.EditorPlacedObjects.ToString();
		string removed = statistics.EditorRemovedObjects.ToString();
		string controlled = statistics.CharactersControlled.ToString();
		string edited = statistics.CharactersEdited.ToString();
		string edit_time = time.Format();
		if (global) {
			placed = GetDayZGame().LoginCache.GlobalItemsPlaced;
			removed = GetDayZGame().LoginCache.GlobalItemsDeleted;
			controlled = GetDayZGame().LoginCache.GlobalCharactersControlled;
			edited = GetDayZGame().LoginCache.GlobalCharactersEdited;
			edit_time = GetDayZGame().LoginCache.GlobalTimePlayed;
		}
		
		m_TemplateController.StatisticsEntries.Clear();
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Placed Objects", placed, global));
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Removed Objects", removed, global));

		if (!global) {
			m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Distance Travelled", string.Format("%1km", statistics.DistanceFlown / 1000), global));
		}
		
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Characters Controlled", controlled, global));
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Characters Edited", edited, global));
		m_TemplateController.StatisticsEntries.Insert(new EditorStatisticsEntryView("Time Edited", edit_time, global));
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
		
		Payload_EditorLoginResponse login_cache = GetDayZGame().LoginCache;
		if (m_IsShowcaseActive && login_cache && GetWidgetUnderCursor() != ServerShowcase) {
			m_ShowcaseTime += dt;
			auto showcase = login_cache.Showcases[m_ShowcaseIndex];
			if (showcase && m_ShowcaseTime > 2.0 && m_ShowcasesReported.Find(m_ShowcaseIndex) == -1 && m_ValidShowcaseSlots.IsValidIndex(m_ShowcaseIndex) && m_ValidShowcaseSlots[m_ShowcaseIndex]) {
				string payload, error;
				Payload_ServerShowcaseReport report_payload = new Payload_ServerShowcaseReport();
				report_payload.Identifier = showcase.Identifier;
				report_payload.Reporter = GetGame().GetUserManager().GetTitleInitiator().GetUid();
				if (JsonFileLoader<Payload_ServerShowcaseReport>.MakeData(report_payload, payload, error)) {
					RestContext ctx = GetRestApi().GetRestContext(Editor.WEB_API_ENDPOINT);
					ctx.SetHeader("application/json\r\nUser-Agent: DayZ-Editor");
					ctx.POST(new RestCallbackBase(), "api\/showcase\/report-impression", payload);
				}

				m_ShowcasesReported.Insert(m_ShowcaseIndex);
			}

			if (showcase && m_ShowcaseTime > 10.0) {
				m_ShowcaseIndex = Math.Rollover(m_ShowcaseIndex + 1, 0, login_cache.Showcases.Count());
				
				if (m_ValidShowcaseSlots.IsValidIndex(m_ShowcaseIndex) && m_ValidShowcaseSlots[m_ShowcaseIndex]) {
					ServerShowcaseImage.SetImage(m_ShowcaseIndex);
					ServerShowcaseImage.Show(true);
					m_ShowcaseTime = 0;
				} else {
					ServerShowcaseImage.Show(false);
				}
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
			
			case GlobeButton: {
				GlobeButton.SetImage(3);
				break;
			}
			
			case SoundButton: {
				SoundButton.SetImage(3);
				break;
			}
			
			case MusicButton: {
				MusicButton.SetImage(3);
				break;
			}

			case ServerShowcase: {
				ServerShowcaseOutline.SetColor(EditorColors.BLUE);
				Payload_EditorLoginResponse login_cache = GetDayZGame().LoginCache;
				if (m_IsShowcaseActive && login_cache && login_cache.Showcases.IsValidIndex(m_ShowcaseIndex)) {
					auto showcase = login_cache.Showcases[m_ShowcaseIndex];
					GetDayZGame().CreateDelayedTooltip(w, showcase.RedirectUrl, TooltipPosition.TOP_RIGHT);
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
			
			case GlobeButton: {
				GlobeButton.SetImage(2);
				break;
			}
			
			case SoundButton: {
				SoundButton.SetImage(2);
				break;
			}
			
			case MusicButton: {
				MusicButton.SetImage(2);
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
		
		Payload_EditorLoginResponse login_cache = GetDayZGame().LoginCache;
		switch (w) {
			case ServerShowcase: {
				if (m_IsShowcaseActive && login_cache) {
					auto showcase = login_cache.Showcases[m_ShowcaseIndex];
					if (showcase) {
						GetGame().OpenURL(showcase.RedirectUrl);
					}
				}
				else {
					GetGame().OpenURL("https:\/\/discord.gg\/dayz-editor");
				}

				break;
			}
			
			case SoundButton: {
				float sound_volume = GetGame().GetSoundScene().GetSoundVolume();
				if (sound_volume) {
					m_SoundVolume = sound_volume;
					GetGame().GetSoundScene().SetSoundVolume(0.0, 0.0);
					Symbols.VOLUME_SLASH.Load(SoundButton);
				} else {
					GetGame().GetSoundScene().SetSoundVolume(m_SoundVolume, 0.0);
					Symbols.VOLUME.Load(SoundButton);
				}
				
				break;
			}
			
			case MusicButton: {
				float music_volume = GetGame().GetSoundScene().GetMusicVolume();
				if (music_volume) {
					m_MusicVolume = music_volume;
					GetGame().GetSoundScene().SetMusicVolume(0.0, 1.0);
					Symbols.MUSIC_SLASH.Load(MusicButton);
				} else {
					GetGame().GetSoundScene().SetMusicVolume(m_MusicVolume, 0.0);
					Symbols.MUSIC.Load(MusicButton);
				}
				
				break;
			}
			
			case GlobeButton: {
				SetStatisticsMode(!m_GlobalStatsVisible);
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
		
		Payload_EditorLoginResponse login_cache = GetDayZGame().LoginCache;

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
				m_ShowcaseIndex = Math.Rollover(m_ShowcaseIndex + 1, 0, login_cache.Showcases.Count());
				if (m_ValidShowcaseSlots.IsValidIndex(m_ShowcaseIndex) && m_ValidShowcaseSlots[m_ShowcaseIndex]) {
					ServerShowcaseImage.SetImage(m_ShowcaseIndex);
					ServerShowcaseImage.Show(true);
					m_ShowcaseTime = 0;
				} else {
					ServerShowcaseImage.Show(false);
				}
			
				break;
			}

			case PrevServerShowcase: {
				m_ShowcaseIndex = Math.Rollover(m_ShowcaseIndex - 1, 0, login_cache.Showcases.Count());
				if (m_ValidShowcaseSlots.IsValidIndex(m_ShowcaseIndex) && m_ValidShowcaseSlots[m_ShowcaseIndex]) {
					ServerShowcaseImage.SetImage(m_ShowcaseIndex);
					ServerShowcaseImage.Show(true);
					m_ShowcaseTime = 0;
				} else {
					ServerShowcaseImage.Show(false);
				}
			
				break;
			}
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		switch (code)
		{
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