class EditorMapView: ScriptView
{
	static ref array<EditorMapView> s_AllEditorMapViews = {};

	protected float m_MotionSicknessDt, m_SoundEffectDt;
	protected string m_MapConfig, m_ImageNormal, m_ImageHover;
	protected AbstractWave m_CurrentMapSound;
	ImageWidget Image;
	TextWidget Text;
	Widget Outline, ButtonSpacer, ImageFr, LockImage, TextPanel;
	ButtonWidget StartButton, LoadButton, PurchaseButton, Button;
	protected int m_Opened;
	
	protected EditorSettings m_EditorSettings;
	protected ref EditorFileDialog m_LoadFileDialog;

	void EditorMapView(string map_config)
	{
		if (!s_AllEditorMapViews) {
			s_AllEditorMapViews = {};
		}

		s_AllEditorMapViews.Insert(this);

		m_MapConfig = map_config;
		m_ImageNormal = GetGame().ConfigGetTextOut(string.Format("CfgWorlds %1 mapTextureClosed", m_MapConfig));
		m_ImageHover = GetGame().ConfigGetTextOut(string.Format("CfgWorlds %1 mapTextureOpened", m_MapConfig));
		string map_name = GetGame().ConfigGetTextOut(string.Format("CfgWorlds %1 description", m_MapConfig));
		Text.SetText(map_name);
		Image.LoadImageFile(0, m_ImageNormal);
		Image.LoadImageFile(1, m_ImageHover);
		Image.SetImage(1);

		m_EditorSettings = EditorSettings.Cast(GetDayZGame().GetProfileSetting(EditorSettings));

		if (!GetGame().VerifyWorldOwnership(m_MapConfig)) {
			PurchaseButton.Show(true);
			StartButton.Show(false);
			LoadButton.Show(false);
			Button.Enable(false);
			LockImage.Show(true);
			Image.SetColor(LinearColor.Create(75, 75, 75));
			Text.SetText(string.Format("%1", map_name));
		} else {
			PurchaseButton.Show(false);
		}
	}

	void ~EditorMapView()
	{
		if (s_AllEditorMapViews) {
			s_AllEditorMapViews.RemoveItem(this);
		}
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
				
		if (!m_LayoutRoot) {
			return;
		}
		
		array<Widget> buttons = { StartButton, LoadButton, PurchaseButton };
		foreach (Widget button: buttons) {
			if (button && button.IsVisible() && (button.GetFlags() & WidgetFlags.DISABLED) == WidgetFlags.DISABLED) {
				button.GetChildren().SetColor(LinearColor.Create(150, 200, 200, 200));
			}
		}

		m_SoundEffectDt -= dt;
		Widget widget_under_cursor = GetWidgetUnderCursor();
		
		int mouse_x, mouse_y, screen_x, screen_y;
	    GetMousePos(mouse_x, mouse_y);
	    GetScreenSize(screen_x, screen_y);
		
		if (widget_under_cursor == ImageFr) {
			float layout_s_x, layout_s_y;
			m_LayoutRoot.GetScreenPos(layout_s_x, layout_s_y);
			
			float i_s_w, i_s_h;
			Image.GetScreenSize(i_s_w, i_s_h);
			
			m_MotionSicknessDt += dt;
			if (m_MotionSicknessDt > (1 / 60)) {			
				const float PARALLAX_AMOUNT = 4.5;
		       // float x_relative = (((mouse_x - layout_s_x) / i_s_w) * PARALLAX_AMOUNT) - (PARALLAX_AMOUNT / 2); // 5.0 is from my extra percent size / 2
		       // float y_relative = (((mouse_y - layout_s_y) / i_s_h) * PARALLAX_AMOUNT) - (PARALLAX_AMOUNT / 2);
				
		        //Image.SetPos(x_relative, y_relative);
				m_MotionSicknessDt = 0;
			}
		}
	}

	void SetOpened(bool state)
	{
		m_Opened = state;
		if (m_Opened) {
			WidgetAnimator.Animate(ButtonSpacer, WidgetAnimatorProperty.POSITION_Y, 0, 60);
			WidgetAnimator.AnimateColor(Outline, m_EditorSettings.SelectionColor, 60);
		} else {
			WidgetAnimator.Animate(ButtonSpacer, WidgetAnimatorProperty.POSITION_Y, -0.2, 50);
			if (GetWidgetUnderCursor() != ImageFr) {
				WidgetAnimator.Animate(Image, WidgetAnimatorProperty.SIZE_H, 1.0, 60);
				WidgetAnimator.Animate(Image, WidgetAnimatorProperty.SIZE_W, 1.0, 60);
				WidgetAnimator.AnimateColor(Image, LinearColor.Create(150, 150, 150), 200);
				WidgetAnimator.AnimateColor(Outline, 0xff191919, 60);
			} else {
				WidgetAnimator.AnimateColor(Outline, m_EditorSettings.HighlightColor, 60);
			}
		}

		Widget child = ButtonSpacer.GetChildren();
		while (child) {
			if (!m_Opened) {
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(child.SetFlags, 0, 0, WidgetFlags.IGNOREPOINTER, 1);
			} else {
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(child.ClearFlags, 200, 0, WidgetFlags.IGNOREPOINTER, 1);
			}

			child = child.GetSibling();
		}
	}

	bool IsOpened()
	{
		return m_Opened;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		bool disabled = ((w.GetFlags() & WidgetFlags.DISABLED) == WidgetFlags.DISABLED);
		
		if (button != 0 || disabled) {
			return super.OnMouseButtonDown(w, x, y, button);
		}
		
		switch (w) {
		}

		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{		
		switch (w) {
			case StartButton: {
				GetGame().PlayMission(CreateEditorMissionFolder(m_MapConfig));
				break;
			}

			case LoadButton: {
				m_LoadFileDialog = new EditorFileDialog(EditorDZEFile, ScriptCaller.Create(OnFileSelected), eDialogMode.OPEN, eDialogFlags.ALLOW_DOUBLE_CLICK, string.Empty);
				break;
			}

			case PurchaseButton: {
				GetGame().GoBuyWorldDLC(m_MapConfig);
				break;
			}
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		bool disabled = ((w.GetFlags() & WidgetFlags.DISABLED) == WidgetFlags.DISABLED);
		if (disabled || button != 0) {
			return super.OnDoubleClick(w, x, y, button);
		}
		
		switch (w) {
			case Button: {
				GetGame().PlayMission(CreateEditorMissionFolder(m_MapConfig));
				break;
			}
		}
		
		return super.OnDoubleClick(w, x, y, button);
	}
	
	protected void OnFileSelected(string file)
	{
		if (!File.Exists(file)) {
			return;
		}
		
		GetDayZGame().EditorFileToLoad = file;
		GetGame().PlayMission(CreateEditorMissionFolder(m_MapConfig));
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{		
		string map_name = GetGame().ConfigGetTextOut(string.Format("CfgWorlds %1 description", m_MapConfig));
		bool disabled = ((w.GetFlags() & WidgetFlags.DISABLED) == WidgetFlags.DISABLED);
		if (disabled) {
			return true;
		}
		
		Widget child_icon = w.FindAnyWidget(string.Format("%1_Icon", w.GetName()));
		ImageWidget child_image = ImageWidget.Cast(child_icon);
		if (child_image && w.IsInherited(ButtonWidget)) {
			WidgetAnimator.Animate(child_image, WidgetAnimatorProperty.SIZE_H, 1.0, 90);
			WidgetAnimator.Animate(child_image, WidgetAnimatorProperty.SIZE_W, 1.0, 90);
			child_image.SetColor(m_EditorSettings.SelectionColor);
			child_image.SetImage(3);
		}
		
		switch (w) {
			case Button: {
				Image.SetImage(1);
	
				WidgetAnimator.Animate(Image, WidgetAnimatorProperty.SIZE_H, 0.95, 120);
				WidgetAnimator.Animate(Image, WidgetAnimatorProperty.SIZE_W, 0.95, 120);
				
				WidgetAnimator.AnimateColor(Image, -1, 200);
				WidgetAnimator.AnimateColor(Text, -1, 100);
				WidgetAnimator.AnimateColor(Outline, m_EditorSettings.SelectionColor, 60);
				WidgetAnimator.AnimateColor(m_LayoutRoot, m_EditorSettings.SelectionColor, 60);
	
				StartButton.GetChildren().SetColor(-1);
				LoadButton.GetChildren().SetColor(-1);
				PurchaseButton.GetChildren().SetColor(-1);
	
				if (!m_Opened && Image.GetColor() != -1) {
					EffectSound snd;
					Camera.GetCurrentCamera().PlaySoundSet(snd, "mapOut_SoundSet", 0, 0);
					snd.SetLocalPosition(Vector(1, 0, 0));
					m_SoundEffectDt = 1.0;
				}
				
				break;
			}
			
			case StartButton: {
				GetDayZGame().CreateDelayedTooltip(w, string.Format("Load %1", map_name), TooltipPosition.TOP_RIGHT);
				break;
			}
			
			case LoadButton: {
				GetDayZGame().CreateDelayedTooltip(w, string.Format("Open Existing File", map_name), TooltipPosition.TOP_RIGHT);
				break;
			}
			
			case PurchaseButton: {
				GetDayZGame().CreateDelayedTooltip(w, string.Format("Purchase %1", map_name), TooltipPosition.TOP_RIGHT);
				break;
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		bool disabled = ((w.GetFlags() & WidgetFlags.DISABLED) == WidgetFlags.DISABLED);
		if (disabled) {
			return true;
		}
		
		GetDayZGame().ClearTooltip();
		
		switch (w) {
			case Button: {
				Image.SetImage(1);
				
				if (enterW != StartButton && enterW != LoadButton && enterW != PurchaseButton) {
					WidgetAnimator.Animate(Image, WidgetAnimatorProperty.SIZE_H, 1.0, 60);
					WidgetAnimator.Animate(Image, WidgetAnimatorProperty.SIZE_W, 1.0, 60);
					WidgetAnimator.AnimateColor(Image, LinearColor.Create(150, 150, 150), 200);
					WidgetAnimator.AnimateColor(Text, LinearColor.Create(150, 150, 150), 100);
					WidgetAnimator.AnimateColor(Outline, 0xff191919, 60);
					WidgetAnimator.AnimateColor(m_LayoutRoot, 0xff191919, 60);
				}

				break;
			}

			default: {
				if (w.IsInherited(ButtonWidget) && w.IsVisible() && (w.GetFlags() & WidgetFlags.DISABLED) != WidgetFlags.DISABLED) {
					ImageWidget.Cast(w.GetChildren()).SetImage(2);
					WidgetAnimator.AnimateColor(w.GetChildren(), -1, 60);
					WidgetAnimator.Animate(w.GetChildren(), WidgetAnimatorProperty.SIZE_H, 0.75, 60);
					WidgetAnimator.Animate(w.GetChildren(), WidgetAnimatorProperty.SIZE_W, 0.75, 60);
				}
				
				break;
			}
		}
		
		return true;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorMapView.layout";
	}
}