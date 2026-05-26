enum EditorAssetsBrowserNavButtonType
{
	EDITOR_ASSETS_BROWSER_NAV_TAB = 0,
	EDITOR_ASSETS_BROWSER_NAV_SUBCATEGORY = 1
}

class EditorAssetsBrowserNavButton: ScriptView
{
	static const float MIN_WIDTH = 82;
	static const float HEIGHT = 26;
	static const float HORIZONTAL_PADDING = 24;

	protected EditorAssetsBrowserView m_Owner;
	protected string m_Id;
	protected int m_ButtonType;
	protected bool m_IsSelected;
	protected bool m_IsHovered;

	Widget AssetsBrowserNavButtonRoot;
	ButtonWidget AssetsBrowserNavButton;
	TextWidget AssetsBrowserNavButtonLabel;

	void EditorAssetsBrowserNavButton(EditorAssetsBrowserView owner, string label, int button_type, string id = string.Empty)
	{
		m_Owner = owner;
		m_Id = id;
		m_ButtonType = button_type;

		AssetsBrowserNavButtonLabel.SetText(label);
		ResizeToContent();
	}

	void SetSelected(bool selected)
	{
		m_IsSelected = selected;
		UpdateVisualState();
	}

	protected void UpdateVisualState()
	{
		if (m_IsSelected) {
			AssetsBrowserNavButtonRoot.SetColor(GetEditor().GetSettings().SelectionColor);
			AssetsBrowserNavButtonLabel.SetColor(ARGB(255, 255, 255, 255));
		} else if (m_IsHovered) {
			AssetsBrowserNavButtonRoot.SetColor(GetEditor().GetSettings().HighlightColor);
			AssetsBrowserNavButtonLabel.SetColor(ARGB(255, 255, 255, 255));
		} else {
			AssetsBrowserNavButtonRoot.SetColor(ARGB(230, 43, 46, 52));
			AssetsBrowserNavButtonLabel.SetColor(ARGB(235, 201, 208, 216));
		}
	}

	string GetId()
	{
		return m_Id;
	}

	int GetButtonType()
	{
		return m_ButtonType;
	}

	void ResizeToContent()
	{
		int label_width, label_height;
		AssetsBrowserNavButtonLabel.GetTextSize(label_width, label_height);

		float button_width = label_width + HORIZONTAL_PADDING;
		if (button_width < MIN_WIDTH) {
			button_width = MIN_WIDTH;
		}

		AssetsBrowserNavButtonRoot.SetSize(button_width, HEIGHT);
		AssetsBrowserNavButton.SetSize(button_width, HEIGHT);
	}

	void SetWidth(float width)
	{
		AssetsBrowserNavButtonRoot.SetSize(width, HEIGHT);
		AssetsBrowserNavButton.SetSize(width, HEIGHT);
	}

	float GetWidth()
	{
		float width, height;
		AssetsBrowserNavButtonRoot.GetSize(width, height);
		return width;
	}

	float GetHeight()
	{
		float width, height;
		AssetsBrowserNavButtonRoot.GetSize(width, height);
		return height;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == AssetsBrowserNavButton && button == MouseState.LEFT) {
			m_Owner.OnNavButtonPressed(this);
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (IsPartOfButton(w)) {
			m_IsHovered = true;
			UpdateVisualState();
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (IsPartOfButton(w) && !IsPartOfButton(enterW)) {
			m_IsHovered = false;
			UpdateVisualState();
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/gui/layouts/assetsbrowser/EditorAssetsBrowserNavButton.layout";
	}

	protected override bool UseUpdateLoop()
	{
		return false;
	}

	protected bool IsPartOfButton(Widget widget)
	{
		Widget current = widget;
		while (current) {
			if (current == GetLayoutRoot()) {
				return true;
			}

			current = current.GetParent();
		}

		return false;
	}
}

class EditorAssetsBrowserSourceFilterRow: ScriptView
{
	protected EditorAssetsBrowserView m_Owner;
	protected string m_SourceId;
	protected bool m_SuppressChanges;

	CheckBoxWidget AssetsBrowserSourceFilterRowCheck;
	TextWidget AssetsBrowserSourceFilterRowLabel;

	void EditorAssetsBrowserSourceFilterRow(EditorAssetsBrowserView owner, string source_id, string source_label)
	{
		m_Owner = owner;
		m_SourceId = source_id;
		AssetsBrowserSourceFilterRowLabel.SetText(source_label);
	}

	string GetSourceId()
	{
		return m_SourceId;
	}

	void SetState(bool checked, bool enabled)
	{
		m_SuppressChanges = true;
		AssetsBrowserSourceFilterRowCheck.SetChecked(checked);
		AssetsBrowserSourceFilterRowCheck.Enable(enabled);
		m_SuppressChanges = false;

		if (AssetsBrowserSourceFilterRowLabel) {
			if (enabled) {
				AssetsBrowserSourceFilterRowLabel.SetColor(ARGB(235, 201, 208, 216));
			} else {
				AssetsBrowserSourceFilterRowLabel.SetColor(ARGB(180, 122, 128, 136));
			}
		}
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w == AssetsBrowserSourceFilterRowCheck && !m_SuppressChanges) {
			m_Owner.OnSourceFilterRowChanged(this, AssetsBrowserSourceFilterRowCheck.IsChecked());
			return true;
		}

		return super.OnChange(w, x, y, finished);
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/gui/layouts/assetsbrowser/EditorAssetsBrowserSourceFilterRow.layout";
	}

	protected override bool UseUpdateLoop()
	{
		return false;
	}
}

class EditorAssetsBrowserPreviewPool: Managed
{
	static const int MAX_ACTIVE_PREVIEWS = 64;
	protected ref array<ref EditorAssetsBrowserCard> m_ActiveCards = {};

	void Refresh(ScrollWidget scroll_widget, array<ref EditorAssetsBrowserCard> cards)
	{
		for (int i = m_ActiveCards.Count() - 1; i >= 0; i--) {
			EditorAssetsBrowserCard active_card = m_ActiveCards[i];
			if (!active_card || cards.Find(active_card) == -1 || !active_card.CanUsePreview(scroll_widget)) {
				if (active_card) {
					active_card.ReleasePreview();
				}

				m_ActiveCards.Remove(i);
			}
		}

		foreach (EditorAssetsBrowserCard card: cards) {
			if (!card) {
				continue;
			}

			if (m_ActiveCards.Find(card) != -1) {
				continue;
			}

			if (!card.CanUsePreview(scroll_widget)) {
				continue;
			}

			if (m_ActiveCards.Count() >= MAX_ACTIVE_PREVIEWS) {
				continue;
			}

			if (card.ActivatePreview()) {
				m_ActiveCards.Insert(card);
			}
		}

		foreach (EditorAssetsBrowserCard position_card: m_ActiveCards) {
			if (position_card) {
				position_card.UpdatePreviewPosition();
			}
		}
	}

	void Clear()
	{
		foreach (EditorAssetsBrowserCard active_card: m_ActiveCards) {
			if (active_card) {
				active_card.ReleasePreview();
			}
		}

		m_ActiveCards.Clear();
	}
}

class EditorAssetsBrowserWidgetFrame: Managed
{
	float X;
	float Y;
	float Width;
	float Height;

	void EditorAssetsBrowserWidgetFrame(float x = 0, float y = 0, float width = 0, float height = 0)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}
}

class EditorAssetsBrowserCard: ScriptView
{
	static const string TITLE_ELLIPSIS = "...";

	protected EditorAssetsBrowserView m_Owner;
	protected EditorAssetsBrowserEntry m_Entry;
	protected EntityAI m_PreviewEntity;
	protected Widget m_PreviewOverlay;
	protected ItemPreviewWidget m_PreviewWidget;
	protected float m_BaseWidth;
	protected float m_BaseHeight;
	protected float m_Width;
	protected float m_Height;
	protected ref EditorAssetsBrowserWidgetFrame m_PreviewFrameLayout;
	protected ref EditorAssetsBrowserWidgetFrame m_FooterLayout;
	protected ref EditorAssetsBrowserWidgetFrame m_TitleLayout;
	protected ref EditorAssetsBrowserWidgetFrame m_SourceLayout;
	protected ref EditorAssetsBrowserWidgetFrame m_FavoriteLayout;
	protected ref EditorAssetsBrowserWidgetFrame m_FavoriteIconLayout;
	protected bool m_IsSelected;
	protected bool m_IsHovered;

	Widget AssetsBrowserCardRoot;
	ButtonWidget AssetsBrowserCardButton;
	Widget AssetsBrowserCardPreviewFrame;
	Widget AssetsBrowserCardFooter;
	ItemPreviewWidget AssetsBrowserCardPreview;
	TextWidget AssetsBrowserCardTitle;
	TextWidget AssetsBrowserCardSource;
	Widget AssetsBrowserCardFavorite;
	ButtonWidget AssetsBrowserCardFavoriteButton;
	ImageWidget AssetsBrowserCardFavoriteIcon;

	void EditorAssetsBrowserCard(EditorAssetsBrowserView owner, EditorAssetsBrowserEntry entry, bool selected)
	{
		m_Owner = owner;
		m_Entry = entry;
		m_IsSelected = selected;
		CaptureLayoutMetrics();
		AssetsBrowserCardFavoriteIcon.LoadImageFile(1, "set:dayz_editor_gui image:StarTicked");

		AssetsBrowserCardTitle.SetText(FitTitleToWidth(entry.DisplayName));
		AssetsBrowserCardSource.SetText(entry.SourceLabel);
		RefreshFavoriteState();
		if (AssetsBrowserCardPreview) {
			AssetsBrowserCardPreview.Show(false);
		}
		UpdateVisualState();
	}

	void ~EditorAssetsBrowserCard()
	{
		ReleasePreview();
	}

	EditorAssetsBrowserEntry GetEntry()
	{
		return m_Entry;
	}

	void SetPosition(float x, float y)
	{
		GetLayoutRoot().SetPos(x, y);
	}

	void SetCardWidth(float width)
	{
		if (m_BaseWidth <= 0 || m_BaseHeight <= 0) {
			CaptureLayoutMetrics();
		}

		if (width <= 0) {
			width = m_BaseWidth;
		}

		float scale = width / m_BaseWidth;
		m_Width = width;
		m_Height = m_BaseHeight * scale;

		GetLayoutRoot().SetSize(m_Width, m_Height);
		ApplyLayoutFrame(AssetsBrowserCardPreviewFrame, m_PreviewFrameLayout, scale);
		ApplyLayoutFrame(AssetsBrowserCardFooter, m_FooterLayout, scale);
		ApplyLayoutFrame(AssetsBrowserCardTitle, m_TitleLayout, scale);
		ApplyLayoutFrame(AssetsBrowserCardSource, m_SourceLayout, scale);
		ApplyLayoutFrame(AssetsBrowserCardFavorite, m_FavoriteLayout, scale);
		ApplyLayoutFrame(AssetsBrowserCardFavoriteIcon, m_FavoriteIconLayout, scale, false);
		AssetsBrowserCardTitle.SetText(FitTitleToWidth(m_Entry.DisplayName));
	}

	float GetBaseWidth()
	{
		return m_BaseWidth;
	}

	float GetBaseHeight()
	{
		return m_BaseHeight;
	}

	float GetWidth()
	{
		return m_Width;
	}

	float GetHeight()
	{
		return m_Height;
	}

	void SetSelected(bool selected)
	{
		m_IsSelected = selected;
		UpdateVisualState();
	}

	protected void UpdateVisualState()
	{
		if (AssetsBrowserCardRoot) {
			if (m_IsHovered) {
				AssetsBrowserCardRoot.SetColor(GetEditor().GetSettings().HighlightColor);
			} else {
				AssetsBrowserCardRoot.SetColor(ARGB(220, 28, 30, 34));
			}
		}

		if (AssetsBrowserCardFooter) {
			AssetsBrowserCardFooter.SetColor(GetFooterColor(m_IsSelected));
		}
	}

	bool CanUsePreview(ScrollWidget scroll_widget)
	{
		if (!m_Entry || !m_Entry.CanPreview || !GetLayoutRoot() || !AssetsBrowserCardPreviewFrame) {
			return false;
		}

		float scroll_x, scroll_y, scroll_w, scroll_h;
		scroll_widget.GetScreenPos(scroll_x, scroll_y);
		scroll_widget.GetScreenSize(scroll_w, scroll_h);

		float preview_x, preview_y, preview_w, preview_h;
		AssetsBrowserCardPreviewFrame.GetScreenPos(preview_x, preview_y);
		AssetsBrowserCardPreviewFrame.GetScreenSize(preview_w, preview_h);

		if (preview_w <= 0 || preview_h <= 0 || scroll_w <= 0 || scroll_h <= 0) {
			return true;
		}

		if (preview_y < scroll_y || preview_y + preview_h > scroll_y + scroll_h) {
			return false;
		}

		if (preview_x + preview_w < scroll_x || preview_x > scroll_x + scroll_w) {
			return false;
		}

		return true;
	}

	bool ActivatePreview()
	{
		if (m_PreviewEntity || !m_Entry || !m_Entry.CanPreview) {
			return m_PreviewEntity != null;
		}

		EditorPlaceableItem preview_placeable = m_Entry.PreviewPlaceable;
		if (!preview_placeable) {
			preview_placeable = m_Entry.Placeable;
		}

		Object preview_object = SpawnPreviewObject(preview_placeable.Type);
		if (!preview_object && preview_placeable.GetSpawnType() != preview_placeable.Type) {
			preview_object = SpawnPreviewObject(preview_placeable.GetSpawnType());
		}

		if (!preview_object) {
			string preview_type = GetEditor().GetObjectManager().ConvertP3dFileToPotentialObjectType(preview_placeable.GetModelName());
			if (preview_type == string.Empty && preview_placeable.Type.Contains(".p3d")) {
				preview_type = GetEditor().GetObjectManager().ConvertP3dFileToPotentialObjectType(preview_placeable.Type);
			}

			if (preview_type != string.Empty) {
				preview_object = SpawnPreviewObject(preview_type);
			}
		}

		// ItemPreviewWidget requires EntityAI, fall back to an EntityAI class when a raw preview object cannot be used directly.
		if (!Class.CastTo(m_PreviewEntity, preview_object)) {
			if (preview_object) {
				string fallback_type = GetEditor().GetObjectManager().ConvertP3dFileToPotentialObjectType(preview_object.GetShapeName());
				if (fallback_type != string.Empty) {
					Object fallback_object = SpawnPreviewObject(fallback_type);
					if (Class.CastTo(m_PreviewEntity, fallback_object)) {
						preview_object.Delete();
					} else if (fallback_object) {
						fallback_object.Delete();
					}
				}

				if (!m_PreviewEntity) {
					preview_object.Delete();
					return false;
				}
			} else {
				return false;
			}
		}

		// ItemPreviewWidget renders only from a root overlay.
		m_PreviewOverlay = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/assetsbrowser/EditorAssetsBrowserPreviewOverlay.layout");
		if (m_PreviewOverlay) {
			m_PreviewWidget = ItemPreviewWidget.Cast(m_PreviewOverlay.FindAnyWidget("AssetsBrowserPreviewItem"));
			if (m_PreviewWidget) {
				PreparePreviewEntity(m_PreviewEntity);
				ConfigurePreviewWidget();
			}
			UpdatePreviewPosition();
		}

		return true;
	}

	protected Object SpawnPreviewObject(string preview_type)
	{
		if (preview_type == string.Empty) {
			return null;
		}

		return GetGame().CreateObjectEx(preview_type, Vector(0, -1000, 0), ECE_LOCAL | ECE_CREATEPHYSICS | ECE_TRACE);
	}

	protected void PreparePreviewEntity(EntityAI preview_entity)
	{
		if (!preview_entity) {
			return;
		}

		dBodyDestroy(preview_entity);
		preview_entity.DisableSimulation(true);
		preview_entity.SetAllowDamage(false);
	}

	protected void ConfigurePreviewWidget()
	{
		if (!m_PreviewWidget || !m_PreviewEntity) {
			return;
		}

		m_PreviewWidget.Show(true);
		m_PreviewWidget.SetItem(m_PreviewEntity);
		m_PreviewWidget.SetView(0);
		m_PreviewWidget.Update();
	}

	void UpdatePreviewPosition()
	{
		if (!m_PreviewOverlay || !AssetsBrowserCardPreviewFrame) {
			return;
		}

		float frame_x, frame_y, frame_w, frame_h;
		AssetsBrowserCardPreviewFrame.GetScreenPos(frame_x, frame_y);
		AssetsBrowserCardPreviewFrame.GetScreenSize(frame_w, frame_h);

		m_PreviewOverlay.SetPos(frame_x, frame_y);
		m_PreviewOverlay.SetSize(frame_w, frame_h);
	}

	void ReleasePreview()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(UpdatePreviewPosition);

		if (m_PreviewWidget) {
			m_PreviewWidget.SetItem(null);
			m_PreviewWidget = null;
		}

		if (m_PreviewOverlay) {
			m_PreviewOverlay.Unlink();
			m_PreviewOverlay = null;
		}

		if (m_PreviewEntity) {
			m_PreviewEntity.Delete();
			m_PreviewEntity = null;
		}
	}

	protected int GetFooterColor(bool selected)
	{
		if (!m_Entry || !m_Entry.Placeable) {
			if (selected) {
				return ARGB(235, 46, 50, 58);
			}

			return ARGB(220, 36, 40, 46);
		}

		if (m_Entry.Placeable.Category == EditorPlaceableItemCategory.STATIC) {
			if (selected) {
				return ARGB(235, 58, 52, 44);
			}

			return ARGB(220, 46, 42, 36);
		}

		if (selected) {
			return ARGB(235, 46, 50, 58);
		}

		return ARGB(220, 36, 40, 46);
	}

	protected void CaptureLayoutMetrics()
	{
		if (!GetLayoutRoot()) {
			return;
		}

		GetLayoutRoot().GetSize(m_BaseWidth, m_BaseHeight);
		m_Width = m_BaseWidth;
		m_Height = m_BaseHeight;
		m_PreviewFrameLayout = ReadLayoutFrame(AssetsBrowserCardPreviewFrame);
		m_FooterLayout = ReadLayoutFrame(AssetsBrowserCardFooter);
		m_TitleLayout = ReadLayoutFrame(AssetsBrowserCardTitle);
		m_SourceLayout = ReadLayoutFrame(AssetsBrowserCardSource);
		m_FavoriteLayout = ReadLayoutFrame(AssetsBrowserCardFavorite);
		m_FavoriteIconLayout = ReadLayoutFrame(AssetsBrowserCardFavoriteIcon);
	}

	protected EditorAssetsBrowserWidgetFrame ReadLayoutFrame(Widget widget)
	{
		EditorAssetsBrowserWidgetFrame frame = new EditorAssetsBrowserWidgetFrame();
		if (!widget) {
			return frame;
		}

		widget.GetPos(frame.X, frame.Y);
		widget.GetSize(frame.Width, frame.Height);
		return frame;
	}

	protected void ApplyLayoutFrame(Widget widget, EditorAssetsBrowserWidgetFrame frame, float scale, bool apply_position = true)
	{
		if (!widget || !frame) {
			return;
		}

		if (apply_position) {
			widget.SetPos(frame.X * scale, frame.Y * scale);
		}

		widget.SetSize(frame.Width * scale, frame.Height * scale);
	}

	void RefreshFavoriteState()
	{
		if (!AssetsBrowserCardFavoriteIcon || !m_Entry) {
			return;
		}

		if (m_Entry.IsFavorite()) {
			AssetsBrowserCardFavoriteIcon.SetImage(1);
			AssetsBrowserCardFavoriteIcon.SetColor(LinearColor.GOLD);
		} else {
			AssetsBrowserCardFavoriteIcon.SetImage(0);
			AssetsBrowserCardFavoriteIcon.SetColor(LinearColor.WHITE);
		}
	}

	protected string FitTitleToWidth(string title)
	{
		if (!AssetsBrowserCardTitle || title == string.Empty) {
			return title;
		}

		title.TrimInPlace();
		if (title == string.Empty) {
			return title;
		}

		float width, height;
		AssetsBrowserCardTitle.GetSize(width, height);
		if (width <= 0) {
			width = m_TitleLayout.Width;
		}

		string candidate = title;
		if (DoesTextFit(AssetsBrowserCardTitle, candidate, width)) {
			return candidate;
		}

		string working_title = title;
		while (working_title.Length() > 1) {
			int trimmed_length = working_title.Length() - 1;
			if (trimmed_length <= 0) {
				break;
			}

			working_title = working_title.Substring(0, trimmed_length);
			working_title.TrimInPlace();
			if (working_title == string.Empty) {
				break;
			}

			candidate = working_title;
			candidate += TITLE_ELLIPSIS;
			if (DoesTextFit(AssetsBrowserCardTitle, candidate, width)) {
				return candidate;
			}
		}

		return TITLE_ELLIPSIS;
	}

	protected bool DoesTextFit(TextWidget widget, string text, float width)
	{
		if (!widget) {
			return false;
		}

		widget.SetText(text);
		widget.Update();

		int text_width, text_height;
		widget.GetTextSize(text_width, text_height);
		return text_width <= width;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == AssetsBrowserCardFavoriteButton && button == MouseState.LEFT) {
			bool favorite = !m_Entry.IsFavorite();
			m_Owner.OnCardFavoriteChanged(this, favorite);
			return true;
		}

		if (w == AssetsBrowserCardButton) {
			if (button == MouseState.LEFT) {
				m_Owner.SelectEntry(m_Entry);
				return true;
			}

			if (button == MouseState.RIGHT) {
				m_Owner.OpenEntryContextMenu(m_Entry, x, y);
				return true;
			}
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (IsPartOfCard(w)) {
			m_IsHovered = true;
			UpdateVisualState();
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (IsPartOfCard(w) && !IsPartOfCard(enterW)) {
			m_IsHovered = false;
			UpdateVisualState();
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/gui/layouts/assetsbrowser/EditorAssetsBrowserCard.layout";
	}

	protected override bool UseUpdateLoop()
	{
		return false;
	}

	protected bool IsPartOfCard(Widget widget)
	{
		Widget current = widget;
		while (current) {
			if (current == GetLayoutRoot()) {
				return true;
			}

			current = current.GetParent();
		}

		return false;
	}
}

class EditorAssetsBrowserSectionHeaderHandler: ScriptedWidgetEventHandler
{
	protected EditorAssetsBrowserSectionView m_Owner;

	void EditorAssetsBrowserSectionHeaderHandler(EditorAssetsBrowserSectionView owner)
	{
		m_Owner = owner;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (!m_Owner) {
			return false;
		}

		return m_Owner.OnHeaderMouseButtonDown(w, x, y, button);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		return m_Owner && m_Owner.OnClick(w, x, y, button);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return m_Owner && m_Owner.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return m_Owner && m_Owner.OnMouseLeave(w, enterW, x, y);
	}
}

class EditorAssetsBrowserSectionView: ScriptView
{
	static const float TITLE_HEIGHT = 22;
	static const float GRID_TOP = 30;
	static const float CARD_GAP = 12;
	static const int MAX_COLUMNS = 9;

	protected EditorAssetsBrowserView m_Owner;
	protected ref EditorAssetsBrowserSectionData m_SectionData;
	protected ref EditorAssetsBrowserSectionHeaderHandler m_HeaderHandler;
	protected string m_SectionKey;
	protected ref array<ref EditorAssetsBrowserCard> m_Cards = {};
	protected float m_Height;
	protected bool m_IsCollapsed;
	protected bool m_IsCollapseHovered;

	Widget AssetsBrowserSectionHeader;
	TextWidget AssetsBrowserSectionTitle;
	Widget AssetsBrowserSectionGrid;
	Widget AssetsBrowserSectionCollapseRoot;
	ButtonWidget AssetsBrowserSectionCollapseButton;
	ImageWidget AssetsBrowserSectionCollapseIcon;

	void EditorAssetsBrowserSectionView(EditorAssetsBrowserView owner, EditorAssetsBrowserSectionData section_data, EditorPlaceableItem selected_placeable, float available_width, string section_key, bool collapsed)
	{
		m_Owner = owner;
		m_SectionData = section_data;
		m_SectionKey = section_key;
		m_IsCollapsed = collapsed;

		AssetsBrowserSectionTitle.SetText(section_data.Label);

		foreach (EditorAssetsBrowserEntry entry: section_data.Entries) {
			EditorAssetsBrowserCard card = new EditorAssetsBrowserCard(owner, entry, entry.Placeable == selected_placeable);
			m_Cards.Insert(card);
			AssetsBrowserSectionGrid.AddChild(card.GetLayoutRoot());
		}

		if (AssetsBrowserSectionCollapseIcon) {
			AssetsBrowserSectionCollapseIcon.LoadImageFile(0, "set:dayz_gui image:icon_expand");
			AssetsBrowserSectionCollapseIcon.LoadImageFile(1, "set:dayz_gui image:icon_collapse");
		}

		UpdateCollapseButton();
		BindHeaderEvents();
		LayoutCards(available_width);
	}

	void ~EditorAssetsBrowserSectionView()
	{
		if (AssetsBrowserSectionHeader) {
			AssetsBrowserSectionHeader.SetHandler(null);
		}

		for (int i = m_Cards.Count() - 1; i >= 0; i--) {
			delete m_Cards[i];
		}
		m_Cards.Clear();
	}

	void AppendCards(out array<ref EditorAssetsBrowserCard> cards)
	{
		if (m_IsCollapsed) {
			return;
		}

		foreach (EditorAssetsBrowserCard card: m_Cards) {
			cards.Insert(card);
		}
	}

	void SetPosition(float x, float y)
	{
		GetLayoutRoot().SetPos(x, y);
	}

	float GetHeight()
	{
		return m_Height;
	}

	bool IsCollapsed()
	{
		return m_IsCollapsed;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == MouseState.LEFT && IsPartOfCollapseButton(w)) {
			m_Owner.ToggleSectionCollapsed(m_SectionKey);
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	bool OnHeaderMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button == MouseState.RIGHT) {
			m_Owner.OpenSectionContextMenu(m_SectionData, x, y);
			return true;
		}

		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (IsPartOfCollapseButton(w)) {
			m_IsCollapseHovered = true;
			UpdateCollapseButton();
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (IsPartOfCollapseButton(w) && !IsPartOfCollapseButton(enterW)) {
			m_IsCollapseHovered = false;
			UpdateCollapseButton();
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}

	protected void BindHeaderEvents()
	{
		if (!AssetsBrowserSectionHeader) {
			return;
		}

		m_HeaderHandler = new EditorAssetsBrowserSectionHeaderHandler(this);
		AssetsBrowserSectionHeader.SetHandler(m_HeaderHandler);
	}

	protected void LayoutCards(float available_width)
	{
		LayoutHeader(available_width);

		EditorAssetsBrowserCard sample_card = null;
		if (m_Cards.Count() > 0) {
			sample_card = m_Cards[0];
		}

		if (m_IsCollapsed || !sample_card) {
			m_Height = TITLE_HEIGHT;
			AssetsBrowserSectionGrid.Show(false);
			AssetsBrowserSectionGrid.SetSize(available_width, 0);
			GetLayoutRoot().SetSize(available_width, m_Height);
			return;
		}

		AssetsBrowserSectionGrid.Show(true);

		float base_card_width = sample_card.GetBaseWidth();
		float base_card_height = sample_card.GetBaseHeight();
		if (base_card_width <= 0 || base_card_height <= 0) {
			return;
		}

		if (available_width < base_card_width) {
			available_width = base_card_width;
		}

		float card_width = base_card_width;
		float max_columns_width = MAX_COLUMNS * base_card_width + (MAX_COLUMNS - 1) * CARD_GAP;
		if (available_width < max_columns_width) {
			card_width = (available_width - (MAX_COLUMNS - 1) * CARD_GAP) / MAX_COLUMNS;
		}

		if (card_width <= 0) {
			card_width = base_card_width;
		}

		float card_height = base_card_height * (card_width / base_card_width);

		int columns = Math.Floor((available_width + CARD_GAP) / (card_width + CARD_GAP));
		if (columns < 1) {
			columns = 1;
		}
		if (columns > MAX_COLUMNS) {
			columns = MAX_COLUMNS;
		}

		for (int i = 0; i < m_Cards.Count(); i++) {
			EditorAssetsBrowserCard card = m_Cards[i];
			if (!card) {
				continue;
			}

			card.SetCardWidth(card_width);

			int column = i % columns;
			int row = i / columns;
			float x = column * (card_width + CARD_GAP);
			float y = row * (card_height + CARD_GAP);
			card.SetPosition(x, y);
		}

		int rows = 0;
		if (m_Cards.Count() > 0) {
			rows = (m_Cards.Count() + columns - 1) / columns;
		}

		float grid_height = 0;
		if (rows > 0) {
			grid_height = rows * card_height + (rows - 1) * CARD_GAP;
		}
		AssetsBrowserSectionGrid.SetSize(available_width, grid_height);
		m_Height = GRID_TOP + grid_height;
		if (grid_height <= 0) {
			m_Height = TITLE_HEIGHT;
		}

		GetLayoutRoot().SetSize(available_width, m_Height);
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/gui/layouts/assetsbrowser/EditorAssetsBrowserSection.layout";
	}

	protected override bool UseUpdateLoop()
	{
		return false;
	}

	protected void LayoutHeader(float available_width)
	{
		if (AssetsBrowserSectionHeader) {
			AssetsBrowserSectionHeader.SetSize(available_width, TITLE_HEIGHT);
		}

		if (AssetsBrowserSectionCollapseRoot) {
			AssetsBrowserSectionCollapseRoot.SetPos(available_width - TITLE_HEIGHT, 0);
			AssetsBrowserSectionCollapseRoot.SetSize(TITLE_HEIGHT, TITLE_HEIGHT);
		}

		if (AssetsBrowserSectionCollapseButton) {
			AssetsBrowserSectionCollapseButton.SetSize(TITLE_HEIGHT, TITLE_HEIGHT);
		}

		if (AssetsBrowserSectionCollapseIcon) {
			AssetsBrowserSectionCollapseIcon.SetSize(TITLE_HEIGHT, TITLE_HEIGHT);
		}

		if (AssetsBrowserSectionTitle) {
			AssetsBrowserSectionTitle.SetSize(available_width - 36, TITLE_HEIGHT);
		}
	}

	protected void UpdateCollapseButton()
	{
		if (AssetsBrowserSectionCollapseRoot) {
			if (m_IsCollapseHovered) {
				AssetsBrowserSectionCollapseRoot.SetColor(GetEditor().GetSettings().HighlightColor);
			} else {
				AssetsBrowserSectionCollapseRoot.SetColor(ARGB(240, 32, 36, 45));
			}
		}

		if (AssetsBrowserSectionCollapseIcon) {
			if (m_IsCollapsed) {
				AssetsBrowserSectionCollapseIcon.SetImage(0);
			} else {
				AssetsBrowserSectionCollapseIcon.SetImage(1);
			}

			AssetsBrowserSectionCollapseIcon.SetColor(ARGB(235, 201, 208, 216));
		}
	}

	protected bool IsPartOfCollapseButton(Widget widget)
	{
		Widget current = widget;
		while (current) {
			if (current == AssetsBrowserSectionCollapseButton || current == AssetsBrowserSectionCollapseRoot) {
				return true;
			}

			current = current.GetParent();
		}

		return false;
	}
}

class EditorAssetsBrowserView: ScriptView
{
	static const float FILTER_BUTTON_SPACING = 6;
	static const float FILTER_ROW_VERTICAL_PADDING = 4;
	static const float SECTION_SPACING = 16;
	static const float COLLAPSED_SECTION_SPACING = 8;
	static const float SOURCE_FILTER_POPUP_WIDTH = 212;
	static const float SOURCE_FILTER_POPUP_PADDING = 8;
	static const float SOURCE_FILTER_ROW_HEIGHT = 24;
	static const float SOURCE_FILTER_BUTTON_HEIGHT = 34;
	static const float SOURCE_FILTER_BUTTON_GAP = 4;
	static const float SOURCE_FILTER_POPUP_GAP = 6;
	static const int SEARCH_REFRESH_DELAY_MS = 200;
	static const int PERSISTENT_STATE_SAVE_DELAY_MS = 300;

	protected EditorHud m_EditorHud;
	protected Editor m_Editor;
	protected EditorAssetsBrowserCatalog m_Catalog;
	protected ref EditorAssetsBrowserFilterState m_FilterState = new EditorAssetsBrowserFilterState();
	protected ref EditorAssetsBrowserPreviewPool m_PreviewPool = new EditorAssetsBrowserPreviewPool();

	protected ref array<ref EditorAssetsBrowserNavButton> m_TabButtons = {};
	protected ref array<ref EditorAssetsBrowserNavButton> m_SubcategoryButtons = {};
	protected ref array<ref EditorAssetsBrowserSectionView> m_SectionViews = {};
	protected ref array<ref EditorAssetsBrowserSourceFilterRow> m_SourceFilterRows = {};
	protected ref array<string> m_PendingSelectedSourceIds = {};
	protected ref map<string, bool> m_CollapsedSections = new map<string, bool>();

	protected bool m_IsOpen;
	protected bool m_RestoreCursorVisible;
	protected bool m_SuppressControlChanges;
	protected bool m_RenderQueued;
	protected bool m_ResetScrollOnNextRender = true;
	protected bool m_RebuildTabButtonsOnNextRender = true;
	protected bool m_RebuildSubcategoryButtonsOnNextRender = true;
	protected bool m_IsSourceFilterPopupOpen;
	protected bool m_IsSourceFilterHovered;
	protected bool m_IsFavoritesOnlyHovered;
	protected bool m_IsConfigOnlyHovered;
	protected bool m_IsStaticOnlyHovered;
	protected bool m_IsSourceFilterConsoleHovered;
	protected bool m_IsSourceFilterPCHovered;
	protected bool m_IsSourceFilterRevertHovered;
	protected bool m_IsSourceFilterSaveHovered;
	protected float m_PreviewRefreshTimer;
	protected float m_LastScrollPosition = -1;
	protected string m_SearchText;
	protected string m_SavedTabId;
	protected string m_SavedSubcategoryId;
	protected int m_PendingPlatformFilter;
	protected int m_SavedPlatformFilter;
	protected int m_SavedPlacementTypeFilter;
	protected bool m_SavedFavoritesOnly;
	protected ref array<string> m_SavedSelectedSourceIds = {};

	Widget AssetsBrowserOverlay;
	Widget AssetsBrowserPanel;
	EditBoxWidget AssetsBrowserSearch;
	ButtonWidget AssetsBrowserSearchClearButton;
	ButtonWidget AssetsBrowserCloseButton;
	Widget AssetsBrowserTabRow;
	Widget AssetsBrowserSubcategoryRow;
	ScrollWidget AssetsBrowserScroll;
	Widget AssetsBrowserSectionList;
	Widget AssetsBrowserSourceFilterRoot;
	ImageWidget AssetsBrowserSourceFilterIcon;
	Widget AssetsBrowserFavoritesOnlyRoot;
	ButtonWidget AssetsBrowserFavoritesOnlyButton;
	ImageWidget AssetsBrowserFavoritesOnlyIcon;
	Widget AssetsBrowserConfigOnlyRoot;
	ButtonWidget AssetsBrowserConfigOnlyButton;
	TextWidget AssetsBrowserConfigOnlyLabel;
	Widget AssetsBrowserStaticOnlyRoot;
	ButtonWidget AssetsBrowserStaticOnlyButton;
	TextWidget AssetsBrowserStaticOnlyLabel;
	Widget AssetsBrowserSourceFilterPopupRoot;
	ScrollWidget AssetsBrowserSourceFilterPopupScroll;
	Widget AssetsBrowserSourceFilterPopupList;
	Widget AssetsBrowserSourceFilterPopupDivider;
	Widget AssetsBrowserSourceFilterConsoleRoot;
	ImageWidget AssetsBrowserSourceFilterConsoleIcon;
	Widget AssetsBrowserSourceFilterPCRoot;
	ImageWidget AssetsBrowserSourceFilterPCIcon;
	Widget AssetsBrowserSourceFilterRevertRoot;
	ImageWidget AssetsBrowserSourceFilterRevertIcon;
	Widget AssetsBrowserSourceFilterSaveRoot;
	ImageWidget AssetsBrowserSourceFilterSaveIcon;
	TextWidget AssetsBrowserEmptyLabel;

	void EditorAssetsBrowserView(EditorHud editor_hud, EditorAssetsBrowserCatalog catalog)
	{
		m_EditorHud = editor_hud;
		m_Editor = GetEditor();
		m_Catalog = catalog;

		EditorSettings settings = m_Editor.GetSettings();
		m_FilterState.TabId = settings.AssetsBrowserLastTabId;
		m_FilterState.SubcategoryId = settings.AssetsBrowserLastSubtabId;
		m_FilterState.PlatformFilter = Math.Clamp(settings.AssetsBrowserPlatformFilter, 0, 1);
		m_FilterState.PlacementTypeFilter = Math.Clamp(settings.AssetsBrowserPlacementTypeFilter, 0, 1);
		m_FilterState.FavoritesOnly = settings.AssetsBrowserFavoritesOnly;
		m_FilterState.SelectedSourceIds = CopyStringArray(settings.AssetsBrowserSelectedSources);
		NormalizeAppliedSourceFilters();
		m_FilterState.SetSearchText(m_SearchText);
		UpdateSavedStateSnapshot();

		InitializeSourceFilterIcons();
		UpdateSourceFilterButton();
		UpdateFavoritesOnlyButton();
		UpdatePlacementTypeButtons();
		UpdateSearchClearButton();
		HideSourceFilterPopup();
		Show(false);
	}

	void ~EditorAssetsBrowserView()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(Render);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(ApplySearchText);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(SavePersistentState);
		SavePersistentState();
		CloseSourceFilterPopup();
		ClearSourceFilterRows();
		ClearTabButtons();
		ClearSubcategoryButtons();
		ClearSections();
		m_PreviewPool.Clear();
	}

	bool IsOpen()
	{
		return m_IsOpen;
	}

	EditorAssetsBrowserCatalog GetCatalog()
	{
		return m_Catalog;
	}

	void Open()
	{
		if (m_IsOpen) {
			return;
		}

		m_IsOpen = true;
		m_RestoreCursorVisible = GetGame().GetUIManager().IsCursorVisible();
		delete EditorHud.CurrentMenu;
		m_EditorHud.ClearCurrentTooltip();

		SyncToCurrentPlaceable();
		m_SuppressControlChanges = true;
		AssetsBrowserSearch.SetText(m_SearchText);
		m_SuppressControlChanges = false;
		CloseSourceFilterPopup();
		UpdateSourceFilterButton();
		UpdateFavoritesOnlyButton();
		UpdatePlacementTypeButtons();
		UpdateSearchClearButton();

		Show(true);
		GetGame().GetUIManager().ShowCursor(true);
		SetFocus(AssetsBrowserSearch);
		GetLayoutRoot().Update();
		Render();
	}

	void Close(bool restore_cursor = true)
	{
		if (!m_IsOpen) {
			return;
		}

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(ApplySearchText);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(SavePersistentState);
		m_IsOpen = false;
		CloseSourceFilterPopup();
		m_SearchText = AssetsBrowserSearch.GetText();
		m_FilterState.SetSearchText(m_SearchText);
		SavePersistentState();
		m_PreviewPool.Clear();
		delete EditorHud.CurrentMenu;
		Show(false);
		SetFocus(null);

		if (restore_cursor) {
			m_EditorHud.ShowCursor(m_RestoreCursorVisible);
		}

		m_EditorHud.OnAssetsBrowserClosed();
	}

	override void Update(float dt)
	{
		super.Update(dt);

		if (!m_IsOpen) {
			return;
		}

		UAInput back_input = GetUApi().GetInputByName("UAUIBack");
		if (back_input && back_input.LocalPress()) {
			Close();
			return;
		}

		float scroll_position = AssetsBrowserScroll.GetVScrollPos();
		m_PreviewRefreshTimer -= dt;
		if (m_PreviewRefreshTimer <= 0 || scroll_position != m_LastScrollPosition) {
			if (scroll_position != m_LastScrollPosition) {
				CloseCurrentMenu();
			}

			RefreshPreviews();
		}
	}

	void OnNavButtonPressed(EditorAssetsBrowserNavButton button)
	{
		if (m_IsSourceFilterPopupOpen) {
			CloseSourceFilterPopup();
		}

		switch (button.GetButtonType()) {
			case EditorAssetsBrowserNavButtonType.EDITOR_ASSETS_BROWSER_NAV_TAB: {
				SelectTab(button.GetId());
				break;
			}

			case EditorAssetsBrowserNavButtonType.EDITOR_ASSETS_BROWSER_NAV_SUBCATEGORY: {
				SelectSubcategory(button.GetId());
				break;
			}
		}
	}

	void SelectEntry(EditorAssetsBrowserEntry entry)
	{
		if (!entry || !entry.Placeable) {
			return;
		}

		m_Editor.ClearHand();
		m_Editor.AddInHand(entry.Placeable);
		m_Editor.GetObjectManager().CurrentSelectedItem = entry.Placeable;
		Close();
	}

	void OpenEntryContextMenu(EditorAssetsBrowserEntry entry, int x, int y)
	{
		if (!entry || !entry.Placeable) {
			return;
		}

		m_Editor.GetObjectManager().CurrentSelectedItem = entry.Placeable;
		delete EditorHud.CurrentMenu;
		EditorHud.CurrentMenu = new EditorAssetsBrowserContextMenu(x, y, this, entry);
	}

	void OpenSectionContextMenu(EditorAssetsBrowserSectionData section, int x, int y)
	{
		if (!section) {
			return;
		}

		delete EditorHud.CurrentMenu;
		EditorHud.CurrentMenu = new EditorAssetsBrowserContextMenu(x, y, this, null, section);
	}

	void OnCardFavoriteChanged(EditorAssetsBrowserCard card, bool favorite)
	{
		if (!card || !card.GetEntry() || !card.GetEntry().Placeable) {
			return;
		}

		m_EditorHud.SetFavoriteState(card.GetEntry().Placeable, favorite);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if ((button == MouseState.LEFT || button == MouseState.RIGHT) && EditorHud.CurrentMenu && !IsPartOfCurrentMenu(w)) {
			CloseCurrentMenu();
		}

		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == MouseState.LEFT && m_IsSourceFilterPopupOpen && !IsPartOfSourceFilterPopup(w) && !IsPartOfSourceFilterButton(w)) {
			CloseSourceFilterPopup();
		}

		if (w == AssetsBrowserCloseButton && button == MouseState.LEFT) {
			Close();
			return true;
		}

		if (w == AssetsBrowserSearchClearButton && button == MouseState.LEFT) {
			m_SuppressControlChanges = true;
			AssetsBrowserSearch.SetText(string.Empty);
			m_SuppressControlChanges = false;
			m_SearchText = string.Empty;
			UpdateSearchClearButton();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(ApplySearchText);
			ApplySearchText();
			SetFocus(AssetsBrowserSearch);
			return true;
		}

		if (button == MouseState.LEFT && IsPartOfSourceFilterButton(w)) {
			ToggleSourceFilterPopup();
			return true;
		}

		if (w == AssetsBrowserFavoritesOnlyButton && button == MouseState.LEFT) {
			m_FilterState.FavoritesOnly = !m_FilterState.FavoritesOnly;
			UpdateFavoritesOnlyButton();
			QueuePersistentStateSave();
			QueueRender(false, false, false);
			return true;
		}

		if (w == AssetsBrowserConfigOnlyButton && button == MouseState.LEFT) {
			SetPlacementTypeFilter(EditorAssetsBrowserPlacementTypeFilter.EDITOR_ASSETS_BROWSER_PLACEMENT_TYPE_CONFIG);
			return true;
		}

		if (w == AssetsBrowserStaticOnlyButton && button == MouseState.LEFT) {
			SetPlacementTypeFilter(EditorAssetsBrowserPlacementTypeFilter.EDITOR_ASSETS_BROWSER_PLACEMENT_TYPE_STATIC);
			return true;
		}

		if (button == MouseState.LEFT && IsPartOfSourceFilterConsoleButton(w)) {
			SetPendingPlatformFilter(EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_CONSOLE);
			return true;
		}

		if (button == MouseState.LEFT && IsPartOfSourceFilterPCButton(w)) {
			SetPendingPlatformFilter(EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_PC);
			return true;
		}

		if (button == MouseState.LEFT && IsPartOfSourceFilterRevertButton(w)) {
			RevertPendingSourceFilters();
			return true;
		}

		if (button == MouseState.LEFT && IsPartOfSourceFilterSaveButton(w)) {
			ApplyPendingSourceFilters();
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (!m_IsOpen || !AssetsBrowserScroll || !IsDescendantOfAssetsBrowser(w)) {
			return super.OnMouseWheel(w, x, y, wheel);
		}

		CloseCurrentMenu();
		AssetsBrowserScroll.VScrollStep(wheel * 14);
		RefreshPreviews();
		return true;
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (m_SuppressControlChanges) {
			return false;
		}

		if (w == AssetsBrowserSearch) {
			m_SearchText = AssetsBrowserSearch.GetText();
			UpdateSearchClearButton();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(ApplySearchText);
			if (finished) {
				ApplySearchText();
			} else {
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ApplySearchText, SEARCH_REFRESH_DELAY_MS, false);
			}
			return true;
		}

		return super.OnChange(w, x, y, finished);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (IsPartOfSourceFilterButton(w)) {
			m_IsSourceFilterHovered = true;
			UpdateSourceFilterButton();
		}

		if (IsPartOfFavoritesOnlyButton(w)) {
			m_IsFavoritesOnlyHovered = true;
			UpdateFavoritesOnlyButton();
		}

		if (IsPartOfConfigOnlyButton(w)) {
			m_IsConfigOnlyHovered = true;
			UpdatePlacementTypeButtons();
		}

		if (IsPartOfStaticOnlyButton(w)) {
			m_IsStaticOnlyHovered = true;
			UpdatePlacementTypeButtons();
		}

		if (IsPartOfSourceFilterConsoleButton(w)) {
			m_IsSourceFilterConsoleHovered = true;
			UpdateSourceFilterPopupButtons();
		}

		if (IsPartOfSourceFilterPCButton(w)) {
			m_IsSourceFilterPCHovered = true;
			UpdateSourceFilterPopupButtons();
		}

		if (IsPartOfSourceFilterRevertButton(w)) {
			m_IsSourceFilterRevertHovered = true;
			UpdateSourceFilterPopupButtons();
		}

		if (IsPartOfSourceFilterSaveButton(w)) {
			m_IsSourceFilterSaveHovered = true;
			UpdateSourceFilterPopupButtons();
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (IsPartOfSourceFilterButton(w) && !IsPartOfSourceFilterButton(enterW)) {
			m_IsSourceFilterHovered = false;
			UpdateSourceFilterButton();
		}

		if (IsPartOfFavoritesOnlyButton(w) && !IsPartOfFavoritesOnlyButton(enterW)) {
			m_IsFavoritesOnlyHovered = false;
			UpdateFavoritesOnlyButton();
		}

		if (IsPartOfConfigOnlyButton(w) && !IsPartOfConfigOnlyButton(enterW)) {
			m_IsConfigOnlyHovered = false;
			UpdatePlacementTypeButtons();
		}

		if (IsPartOfStaticOnlyButton(w) && !IsPartOfStaticOnlyButton(enterW)) {
			m_IsStaticOnlyHovered = false;
			UpdatePlacementTypeButtons();
		}

		if (IsPartOfSourceFilterConsoleButton(w) && !IsPartOfSourceFilterConsoleButton(enterW)) {
			m_IsSourceFilterConsoleHovered = false;
			UpdateSourceFilterPopupButtons();
		}

		if (IsPartOfSourceFilterPCButton(w) && !IsPartOfSourceFilterPCButton(enterW)) {
			m_IsSourceFilterPCHovered = false;
			UpdateSourceFilterPopupButtons();
		}

		if (IsPartOfSourceFilterRevertButton(w) && !IsPartOfSourceFilterRevertButton(enterW)) {
			m_IsSourceFilterRevertHovered = false;
			UpdateSourceFilterPopupButtons();
		}

		if (IsPartOfSourceFilterSaveButton(w) && !IsPartOfSourceFilterSaveButton(enterW)) {
			m_IsSourceFilterSaveHovered = false;
			UpdateSourceFilterPopupButtons();
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}

	protected void SelectTab(string tab_id)
	{
		if (m_FilterState.TabId == tab_id) {
			return;
		}

		m_FilterState.TabId = tab_id;
		m_FilterState.SubcategoryId = string.Empty;
		QueuePersistentStateSave();
		QueueRender(true, false, true);
	}

	protected void SelectSubcategory(string subcategory_id)
	{
		if (m_FilterState.SubcategoryId == subcategory_id) {
			return;
		}

		m_FilterState.SubcategoryId = subcategory_id;
		QueuePersistentStateSave();
		QueueRender(true, false, false);
	}

	protected void ToggleSourceFilterPopup()
	{
		if (m_IsSourceFilterPopupOpen) {
			CloseSourceFilterPopup();
			return;
		}

		OpenSourceFilterPopup();
	}

	protected void OpenSourceFilterPopup()
	{
		if (m_IsSourceFilterPopupOpen) {
			return;
		}

		RevertPendingSourceFilters();
		BuildSourceFilterRows();
		UpdateSourceFilterPopup();
		if (AssetsBrowserSourceFilterPopupRoot) {
			AssetsBrowserSourceFilterPopupRoot.Show(true);
		}

		m_IsSourceFilterPopupOpen = true;
		UpdateSourceFilterButton();
	}

	protected void HideSourceFilterPopup()
	{
		if (AssetsBrowserSourceFilterPopupRoot) {
			AssetsBrowserSourceFilterPopupRoot.Show(false);
		}
	}

	protected void CloseSourceFilterPopup()
	{
		if (!m_IsSourceFilterPopupOpen) {
			HideSourceFilterPopup();
			return;
		}

		m_IsSourceFilterConsoleHovered = false;
		m_IsSourceFilterPCHovered = false;
		m_IsSourceFilterRevertHovered = false;
		m_IsSourceFilterSaveHovered = false;
		m_IsSourceFilterPopupOpen = false;
		HideSourceFilterPopup();
		UpdateSourceFilterButton();
	}

	protected void RevertPendingSourceFilters()
	{
		m_PendingPlatformFilter = m_FilterState.PlatformFilter;
		m_PendingSelectedSourceIds = CopyStringArray(m_FilterState.SelectedSourceIds);
		if (m_PendingPlatformFilter == EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_CONSOLE) {
			m_PendingSelectedSourceIds.Clear();
			m_PendingSelectedSourceIds.Insert("vanilla");
		}
		NormalizeSelectedSourceIds(m_PendingSelectedSourceIds, true);
		UpdateSourceFilterPopup();
	}

	protected void SetPendingPlatformFilter(int filter_value)
	{
		if (m_PendingPlatformFilter == filter_value) {
			return;
		}

		m_PendingPlatformFilter = filter_value;
		if (m_PendingPlatformFilter == EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_CONSOLE) {
			m_PendingSelectedSourceIds.Clear();
			m_PendingSelectedSourceIds.Insert("vanilla");
		}
		UpdateSourceFilterPopup();
	}

	protected void ApplyPendingSourceFilters()
	{
		NormalizeSelectedSourceIds(m_PendingSelectedSourceIds);

		bool filters_changed = false;
		if (m_FilterState.PlatformFilter != m_PendingPlatformFilter) {
			filters_changed = true;
		}

		if (!AreStringArraysEqual(m_FilterState.SelectedSourceIds, m_PendingSelectedSourceIds)) {
			filters_changed = true;
		}

		m_FilterState.PlatformFilter = m_PendingPlatformFilter;
		m_FilterState.SelectedSourceIds = CopyStringArray(m_PendingSelectedSourceIds);

		CloseSourceFilterPopup();
		UpdateSourceFilterButton();
		if (!filters_changed) {
			return;
		}

		QueuePersistentStateSave();
		QueueRender(true, false, false);
	}

	void OnSourceFilterRowChanged(EditorAssetsBrowserSourceFilterRow row, bool checked)
	{
		if (!row || m_PendingPlatformFilter == EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_CONSOLE) {
			return;
		}

		string source_id = row.GetSourceId();
		int existing_index = m_PendingSelectedSourceIds.Find(source_id);
		if (checked) {
			if (existing_index == -1) {
				m_PendingSelectedSourceIds.Insert(source_id);
			}
		} else if (existing_index != -1) {
			if (m_PendingSelectedSourceIds.Count() <= 1) {
				UpdateSourceFilterRows();
				return;
			}

			m_PendingSelectedSourceIds.Remove(existing_index);
		}
	}

	protected void NormalizeAppliedSourceFilters()
	{
		NormalizeSelectedSourceIds(m_FilterState.SelectedSourceIds, true);
	}

	protected void NormalizeSelectedSourceIds(array<string> source_ids, bool restore_defaults = false)
	{
		if (!source_ids) {
			return;
		}

		ref array<string> normalized_ids = {};
		array<ref EditorAssetsBrowserSourceData> available_sources = {};
		if (m_Catalog) {
			available_sources = m_Catalog.GetSources();
		}

		foreach (EditorAssetsBrowserSourceData available_source: available_sources) {
			if (!available_source) {
				continue;
			}

			if (source_ids.Find(available_source.Id) != -1) {
				normalized_ids.Insert(available_source.Id);
			}
		}

		if (restore_defaults && normalized_ids.Count() == 0) {
			foreach (EditorAssetsBrowserSourceData default_source: available_sources) {
				if (!default_source) {
					continue;
				}

				normalized_ids.Insert(default_source.Id);
			}
		}

		source_ids.Clear();
		foreach (string normalized_id: normalized_ids) {
			source_ids.Insert(normalized_id);
		}
	}

	protected void BuildSourceFilterRows()
	{
		ClearSourceFilterRows();
		if (!AssetsBrowserSourceFilterPopupList || !m_Catalog) {
			return;
		}

		array<ref EditorAssetsBrowserSourceData> sources = m_Catalog.GetSources();
		foreach (EditorAssetsBrowserSourceData source: sources) {
			if (!source) {
				continue;
			}

			EditorAssetsBrowserSourceFilterRow row = new EditorAssetsBrowserSourceFilterRow(this, source.Id, source.Label);
			m_SourceFilterRows.Insert(row);
			AssetsBrowserSourceFilterPopupList.AddChild(row.GetLayoutRoot());
		}
	}

	protected void ClearSourceFilterRows()
	{
		for (int i = m_SourceFilterRows.Count() - 1; i >= 0; i--) {
			delete m_SourceFilterRows[i];
		}

		m_SourceFilterRows.Clear();
	}

	protected void UpdateSourceFilterPopup()
	{
		UpdateSourceFilterRows();
		LayoutSourceFilterPopup();
		UpdateSourceFilterPopupButtons();
		UpdateSourceFilterButton();
	}

	protected void UpdateSourceFilterRows()
	{
		foreach (EditorAssetsBrowserSourceFilterRow row: m_SourceFilterRows) {
			if (!row) {
				continue;
			}

			bool enabled = true;
			bool checked = m_PendingSelectedSourceIds.Find(row.GetSourceId()) != -1;
			if (m_PendingPlatformFilter == EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_CONSOLE && row.GetSourceId() != "vanilla") {
				enabled = false;
				checked = false;
			}

			row.SetState(checked, enabled);
		}
	}

	protected void LayoutSourceFilterPopup()
	{
		if (!AssetsBrowserSourceFilterPopupRoot || !AssetsBrowserSourceFilterPopupScroll || !AssetsBrowserSourceFilterPopupList || !AssetsBrowserSourceFilterRoot || !AssetsBrowserPanel) {
			return;
		}

		float inner_width = SOURCE_FILTER_POPUP_WIDTH - SOURCE_FILTER_POPUP_PADDING * 2;
		float list_height = m_SourceFilterRows.Count() * SOURCE_FILTER_ROW_HEIGHT;
		float button_width = (inner_width - SOURCE_FILTER_BUTTON_GAP) * 0.5;
		float fixed_height = SOURCE_FILTER_POPUP_PADDING + SOURCE_FILTER_POPUP_GAP + 1 + SOURCE_FILTER_POPUP_GAP + SOURCE_FILTER_BUTTON_HEIGHT + SOURCE_FILTER_BUTTON_GAP + SOURCE_FILTER_BUTTON_HEIGHT + SOURCE_FILTER_POPUP_PADDING;

		float panel_screen_width;
		float panel_screen_height;
		float panel_screen_x;
		float panel_screen_y;
		float button_screen_x;
		float button_screen_y;
		AssetsBrowserPanel.GetScreenSize(panel_screen_width, panel_screen_height);
		AssetsBrowserPanel.GetScreenPos(panel_screen_x, panel_screen_y);
		AssetsBrowserSourceFilterRoot.GetScreenPos(button_screen_x, button_screen_y);

		float max_popup_height = button_screen_y - panel_screen_y - SOURCE_FILTER_POPUP_GAP;
		float max_list_height = max_popup_height - fixed_height;
		if (max_list_height < 0) {
			max_list_height = 0;
		}

		float list_view_height = list_height;
		if (list_view_height > max_list_height) {
			list_view_height = max_list_height;
		}

		AssetsBrowserSourceFilterPopupScroll.SetPos(SOURCE_FILTER_POPUP_PADDING, SOURCE_FILTER_POPUP_PADDING);
		AssetsBrowserSourceFilterPopupScroll.SetSize(inner_width, list_view_height);
		AssetsBrowserSourceFilterPopupList.SetPos(0, 0);
		AssetsBrowserSourceFilterPopupList.SetSize(inner_width, list_height);

		float row_y = 0;
		foreach (EditorAssetsBrowserSourceFilterRow row: m_SourceFilterRows) {
			if (!row) {
				continue;
			}

			row.GetLayoutRoot().SetPos(0, row_y);
			row.GetLayoutRoot().SetSize(inner_width, SOURCE_FILTER_ROW_HEIGHT);
			row_y += SOURCE_FILTER_ROW_HEIGHT;
		}

		float divider_y = SOURCE_FILTER_POPUP_PADDING + list_view_height + SOURCE_FILTER_POPUP_GAP;
		if (AssetsBrowserSourceFilterPopupDivider) {
			AssetsBrowserSourceFilterPopupDivider.SetPos(SOURCE_FILTER_POPUP_PADDING, divider_y);
			AssetsBrowserSourceFilterPopupDivider.SetSize(inner_width, 1);
		}

		float top_buttons_y = divider_y + 1 + SOURCE_FILTER_POPUP_GAP;
		float bottom_buttons_y = top_buttons_y + SOURCE_FILTER_BUTTON_HEIGHT + SOURCE_FILTER_BUTTON_GAP;
		SetSourceFilterPopupButtonLayout(AssetsBrowserSourceFilterConsoleRoot, SOURCE_FILTER_POPUP_PADDING, top_buttons_y, button_width, SOURCE_FILTER_BUTTON_HEIGHT);
		SetSourceFilterPopupButtonLayout(AssetsBrowserSourceFilterPCRoot, SOURCE_FILTER_POPUP_PADDING + button_width + SOURCE_FILTER_BUTTON_GAP, top_buttons_y, button_width, SOURCE_FILTER_BUTTON_HEIGHT);
		SetSourceFilterPopupButtonLayout(AssetsBrowserSourceFilterRevertRoot, SOURCE_FILTER_POPUP_PADDING, bottom_buttons_y, button_width, SOURCE_FILTER_BUTTON_HEIGHT);
		SetSourceFilterPopupButtonLayout(AssetsBrowserSourceFilterSaveRoot, SOURCE_FILTER_POPUP_PADDING + button_width + SOURCE_FILTER_BUTTON_GAP, bottom_buttons_y, button_width, SOURCE_FILTER_BUTTON_HEIGHT);

		float popup_height = bottom_buttons_y + SOURCE_FILTER_BUTTON_HEIGHT + SOURCE_FILTER_POPUP_PADDING;
		AssetsBrowserSourceFilterPopupRoot.SetSize(SOURCE_FILTER_POPUP_WIDTH, popup_height);

		float local_x = button_screen_x - panel_screen_x;
		float local_y = button_screen_y - panel_screen_y - popup_height - 6;
		float max_local_x = panel_screen_width - SOURCE_FILTER_POPUP_WIDTH;
		if (max_local_x < 0) {
			max_local_x = 0;
		}

		if (local_x > max_local_x) {
			local_x = max_local_x;
		}

		if (local_x < 0) {
			local_x = 0;
		}

		if (local_y < 0) {
			local_y = 0;
		}

		float max_local_y = panel_screen_height - popup_height;
		if (max_local_y < 0) {
			max_local_y = 0;
		}

		if (local_y > max_local_y) {
			local_y = max_local_y;
		}

		AssetsBrowserSourceFilterPopupRoot.SetPos(local_x, local_y);
		AssetsBrowserSourceFilterPopupScroll.VScrollToPos(0);
	}

	protected void SetSourceFilterPopupButtonLayout(Widget root, float x, float y, float width, float height)
	{
		if (!root) {
			return;
		}

		root.SetPos(x, y);
		root.SetSize(width, height);
	}

	protected void InitializeSourceFilterIcons()
	{
		if (AssetsBrowserSourceFilterIcon) {
			AssetsBrowserSourceFilterIcon.SetImage(3);
		}

		if (AssetsBrowserSourceFilterConsoleIcon) {
			AssetsBrowserSourceFilterConsoleIcon.SetImage(3);
		}

		if (AssetsBrowserSourceFilterPCIcon) {
			AssetsBrowserSourceFilterPCIcon.SetImage(3);
		}

		if (AssetsBrowserSourceFilterRevertIcon) {
			AssetsBrowserSourceFilterRevertIcon.SetImage(3);
		}

		if (AssetsBrowserSourceFilterSaveIcon) {
			AssetsBrowserSourceFilterSaveIcon.SetImage(3);
		}
	}

	protected void UpdateSourceFilterButton()
	{
		bool selected = m_IsSourceFilterPopupOpen || IsSourceFilterActive();

		if (AssetsBrowserSourceFilterRoot) {
			if (selected) {
				AssetsBrowserSourceFilterRoot.SetColor(GetEditor().GetSettings().SelectionColor);
			} else if (m_IsSourceFilterHovered) {
				AssetsBrowserSourceFilterRoot.SetColor(GetEditor().GetSettings().HighlightColor);
			} else {
				AssetsBrowserSourceFilterRoot.SetColor(ARGB(255, 41, 43, 51));
			}
		}

		if (AssetsBrowserSourceFilterIcon) {
			if (selected || m_IsSourceFilterHovered) {
				AssetsBrowserSourceFilterIcon.SetColor(ARGB(255, 255, 255, 255));
			} else {
				AssetsBrowserSourceFilterIcon.SetColor(ARGB(235, 201, 208, 216));
			}
		}
	}

	protected void UpdateSourceFilterPopupButtons()
	{
		UpdateSourceFilterPopupButton(AssetsBrowserSourceFilterConsoleRoot, AssetsBrowserSourceFilterConsoleIcon, m_IsSourceFilterConsoleHovered, m_PendingPlatformFilter == EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_CONSOLE);
		UpdateSourceFilterPopupButton(AssetsBrowserSourceFilterPCRoot, AssetsBrowserSourceFilterPCIcon, m_IsSourceFilterPCHovered, m_PendingPlatformFilter == EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_PC);
		UpdateSourceFilterPopupButton(AssetsBrowserSourceFilterRevertRoot, AssetsBrowserSourceFilterRevertIcon, m_IsSourceFilterRevertHovered, false);
		UpdateSourceFilterPopupButton(AssetsBrowserSourceFilterSaveRoot, AssetsBrowserSourceFilterSaveIcon, m_IsSourceFilterSaveHovered, false);
	}

	protected void UpdateSourceFilterPopupButton(Widget root, ImageWidget icon, bool hovered, bool selected)
	{
		if (root) {
			if (selected) {
				root.SetColor(GetEditor().GetSettings().SelectionColor);
			} else if (hovered) {
				root.SetColor(GetEditor().GetSettings().HighlightColor);
			} else {
				root.SetColor(ARGB(255, 41, 43, 51));
			}
		}

		if (icon) {
			icon.SetColor(ARGB(255, 255, 255, 255));
		}
	}

	protected bool IsSourceFilterActive()
	{
		if (m_FilterState.PlatformFilter != EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_PC) {
			return true;
		}

		return !AreStringArraysEqual(m_FilterState.SelectedSourceIds, GetAllSourceIds());
	}

	protected ref array<string> GetAllSourceIds()
	{
		ref array<string> source_ids = {};
		if (!m_Catalog) {
			return source_ids;
		}

		array<ref EditorAssetsBrowserSourceData> sources = m_Catalog.GetSources();
		foreach (EditorAssetsBrowserSourceData source: sources) {
			if (!source) {
				continue;
			}

			source_ids.Insert(source.Id);
		}

		return source_ids;
	}

	protected ref array<string> CopyStringArray(array<string> source)
	{
		ref array<string> copy = {};
		if (!source) {
			return copy;
		}

		foreach (string value: source) {
			copy.Insert(value);
		}

		return copy;
	}

	protected bool AreStringArraysEqual(array<string> lhs, array<string> rhs)
	{
		if (!lhs && !rhs) {
			return true;
		}

		if (!lhs || !rhs) {
			return false;
		}

		if (lhs.Count() != rhs.Count()) {
			return false;
		}

		foreach (string value: lhs) {
			if (rhs.Find(value) == -1) {
				return false;
			}
		}

		return true;
	}

	protected void SetPlacementTypeFilter(int filter_value)
	{
		if (m_FilterState.PlacementTypeFilter == filter_value) {
			return;
		}

		m_FilterState.PlacementTypeFilter = filter_value;
		UpdatePlacementTypeButtons();
		QueuePersistentStateSave();
		QueueRender(true, false, false);
	}

	protected void Render()
	{
		m_RenderQueued = false;
		if (!m_IsOpen || !GetLayoutRoot()) {
			return;
		}

		string previous_tab_id = m_FilterState.TabId;
		bool rebuild_tab_buttons = m_RebuildTabButtonsOnNextRender || m_TabButtons.Count() == 0;
		bool rebuild_subcategory_buttons = m_RebuildSubcategoryButtonsOnNextRender || m_SubcategoryButtons.Count() == 0;
		m_RebuildTabButtonsOnNextRender = true;
		m_RebuildSubcategoryButtonsOnNextRender = true;

		m_PreviewPool.Clear();
		ClearSections();

		EnsureValidSelection();
		if (m_FilterState.TabId != previous_tab_id) {
			rebuild_subcategory_buttons = true;
		}

		if (rebuild_tab_buttons) {
			ClearTabButtons();
		}

		if (rebuild_subcategory_buttons) {
			ClearSubcategoryButtons();
		}

		QueuePersistentStateSave();
		GetLayoutRoot().Update();
		AssetsBrowserScroll.Update();
		UpdateSourceFilterButton();
		UpdatePlacementTypeButtons();
		if (rebuild_tab_buttons) {
			BuildTabButtons();
		} else {
			UpdateTabButtonSelection();
		}
		if (rebuild_subcategory_buttons) {
			BuildSubcategoryButtons();
		} else {
			UpdateSubcategoryButtonSelection();
		}
		BuildSections();

		AssetsBrowserSectionList.Update();
		AssetsBrowserScroll.Update();
		if (m_ResetScrollOnNextRender) {
			AssetsBrowserScroll.VScrollToPos(0);
		}
		m_ResetScrollOnNextRender = true;
		RefreshPreviews();
	}

	protected void QueueRender(bool reset_scroll = true, bool rebuild_tab_buttons = true, bool rebuild_subcategory_buttons = true)
	{
		if (reset_scroll) {
			m_ResetScrollOnNextRender = true;
		} else if (!m_RenderQueued) {
			m_ResetScrollOnNextRender = false;
		}

		if (rebuild_tab_buttons) {
			m_RebuildTabButtonsOnNextRender = true;
		}

		if (rebuild_subcategory_buttons) {
			m_RebuildSubcategoryButtonsOnNextRender = true;
		}

		if (m_RenderQueued) {
			return;
		}

		m_RebuildTabButtonsOnNextRender = rebuild_tab_buttons;
		m_RebuildSubcategoryButtonsOnNextRender = rebuild_subcategory_buttons;
		m_RenderQueued = true;
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(Render, 0, false);
	}

	protected void ApplySearchText()
	{
		m_FilterState.SetSearchText(m_SearchText);
		if (m_IsOpen) {
			QueueRender(false, false, false);
		}
	}

	protected void UpdateSearchClearButton()
	{
		if (AssetsBrowserSearchClearButton) {
			AssetsBrowserSearchClearButton.Show(m_SearchText != string.Empty);
		}
	}

	protected void UpdateFavoritesOnlyButton()
	{
		if (AssetsBrowserFavoritesOnlyRoot) {
			if (m_IsFavoritesOnlyHovered) {
				AssetsBrowserFavoritesOnlyRoot.SetColor(GetEditor().GetSettings().HighlightColor);
			} else {
				AssetsBrowserFavoritesOnlyRoot.SetColor(ARGB(255, 41, 43, 51));
			}
		}

		if (AssetsBrowserFavoritesOnlyIcon) {
			if (m_FilterState.FavoritesOnly) {
				AssetsBrowserFavoritesOnlyIcon.SetImage(3);
			} else {
				AssetsBrowserFavoritesOnlyIcon.SetImage(2);
			}

			if (m_FilterState.FavoritesOnly) {
				AssetsBrowserFavoritesOnlyIcon.SetColor(ARGB(255, 255, 255, 255));
			} else {
				AssetsBrowserFavoritesOnlyIcon.SetColor(ARGB(235, 201, 208, 216));
			}
		}
	}

	protected void UpdatePlacementTypeButtons()
	{
		UpdatePlacementTypeButton(AssetsBrowserConfigOnlyRoot, AssetsBrowserConfigOnlyLabel, m_IsConfigOnlyHovered, m_FilterState.PlacementTypeFilter == EditorAssetsBrowserPlacementTypeFilter.EDITOR_ASSETS_BROWSER_PLACEMENT_TYPE_CONFIG);
		UpdatePlacementTypeButton(AssetsBrowserStaticOnlyRoot, AssetsBrowserStaticOnlyLabel, m_IsStaticOnlyHovered, m_FilterState.PlacementTypeFilter == EditorAssetsBrowserPlacementTypeFilter.EDITOR_ASSETS_BROWSER_PLACEMENT_TYPE_STATIC);
	}

	protected void UpdatePlacementTypeButton(Widget root, TextWidget label, bool hovered, bool selected)
	{
		if (root) {
			if (selected) {
				root.SetColor(GetEditor().GetSettings().SelectionColor);
			} else if (hovered) {
				root.SetColor(GetEditor().GetSettings().HighlightColor);
			} else {
				root.SetColor(ARGB(255, 41, 43, 51));
			}
		}

		if (label) {
			if (selected || hovered) {
				label.SetColor(ARGB(255, 255, 255, 255));
			} else {
				label.SetColor(ARGB(235, 201, 208, 216));
			}
		}
	}

	protected void BuildTabButtons()
	{
		array<ref EditorAssetsBrowserTabDefinition> tabs = m_Catalog.GetTaxonomy().GetTabs();
		foreach (EditorAssetsBrowserTabDefinition tab: tabs) {
			CreateTabButton(tab.Id, tab.Label);
		}

		LayoutNavButtons(m_TabButtons, AssetsBrowserTabRow);
	}

	protected void UpdateTabButtonSelection()
	{
		foreach (EditorAssetsBrowserNavButton button: m_TabButtons) {
			if (button) {
				button.SetSelected(button.GetId() == m_FilterState.TabId);
			}
		}
	}

	protected void UpdateSubcategoryButtonSelection()
	{
		foreach (EditorAssetsBrowserNavButton button: m_SubcategoryButtons) {
			if (button) {
				button.SetSelected(button.GetId() == m_FilterState.SubcategoryId);
			}
		}
	}

	protected void BuildSubcategoryButtons()
	{
		array<ref EditorAssetsBrowserSubcategoryDefinition> subcategories = m_Catalog.GetTaxonomy().GetSubcategories();
		foreach (EditorAssetsBrowserSubcategoryDefinition subcat: subcategories) {
			if (subcat.TabId == m_FilterState.TabId) {
				CreateSubcategoryButton(subcat.Id, subcat.Label);
			}
		}

		LayoutNavButtons(m_SubcategoryButtons, AssetsBrowserSubcategoryRow);
	}

	protected void BuildSections()
	{
		EditorPlaceableItem selected_placeable = GetCurrentPlaceable();
		array<ref EditorAssetsBrowserSectionData> sections = m_Catalog.GetSections(m_FilterState);
		float content_width = GetSectionContentWidth();
		float current_y = 0;

		foreach (EditorAssetsBrowserSectionData section: sections) {
			string section_key = GetSectionStateKey(section);
			EditorAssetsBrowserSectionView section_view = new EditorAssetsBrowserSectionView(this, section, selected_placeable, content_width, section_key, IsSectionCollapsed(section_key));
			section_view.SetPosition(0, current_y);
			m_SectionViews.Insert(section_view);
			AssetsBrowserSectionList.AddChild(section_view.GetLayoutRoot());

			if (section_view.IsCollapsed()) {
				current_y += section_view.GetHeight() + COLLAPSED_SECTION_SPACING;
			} else {
				current_y += section_view.GetHeight() + SECTION_SPACING;
			}
		}

		AssetsBrowserSectionList.SetSize(content_width, current_y);
		AssetsBrowserEmptyLabel.Show(sections.Count() == 0);
	}

	protected void EnsureValidSelection()
	{
		EditorAssetsBrowserFilterState tab_scope = CopyState();
		tab_scope.TabId = string.Empty;
		tab_scope.SubcategoryId = string.Empty;

		if (m_FilterState.TabId == string.Empty || m_Catalog.CountEntriesForTab(tab_scope, m_FilterState.TabId) == 0) {
			m_FilterState.TabId = GetFirstVisibleTab(tab_scope);
		}

		if (m_FilterState.TabId == string.Empty) {
			m_FilterState.SubcategoryId = string.Empty;
			return;
		}

		if (m_FilterState.SubcategoryId == string.Empty || m_Catalog.CountEntriesForSubcategory(tab_scope, m_FilterState.TabId, m_FilterState.SubcategoryId) == 0) {
			m_FilterState.SubcategoryId = GetFirstVisibleSubcategory(tab_scope, m_FilterState.TabId);
		}
	}

	protected string GetFirstVisibleTab(EditorAssetsBrowserFilterState scope_state)
	{
		array<ref EditorAssetsBrowserTabDefinition> tabs = m_Catalog.GetTaxonomy().GetTabs();
		foreach (EditorAssetsBrowserTabDefinition tab: tabs) {
			if (m_Catalog.CountEntriesForTab(scope_state, tab.Id) > 0) {
				return tab.Id;
			}
		}

		return string.Empty;
	}

	protected string GetFirstVisibleSubcategory(EditorAssetsBrowserFilterState scope_state, string tab_id)
	{
		array<ref EditorAssetsBrowserSubcategoryDefinition> subcategories = m_Catalog.GetTaxonomy().GetSubcategories();
		foreach (EditorAssetsBrowserSubcategoryDefinition subcat: subcategories) {
			if (subcat.TabId == tab_id && m_Catalog.CountEntriesForSubcategory(scope_state, tab_id, subcat.Id) > 0) {
				return subcat.Id;
			}
		}

		return string.Empty;
	}

	void RefreshAllCardFavorites()
	{
		foreach (EditorAssetsBrowserSectionView section_view: m_SectionViews) {
			ref array<ref EditorAssetsBrowserCard> section_cards = {};
			if (section_view) {
				section_view.AppendCards(section_cards);
				foreach (EditorAssetsBrowserCard card: section_cards) {
					if (card) {
						card.RefreshFavoriteState();
					}
				}
			}
		}
	}

	void OnFavoriteStateChanged()
	{
		if (m_IsOpen && m_FilterState.FavoritesOnly) {
			QueueRender(false, false, false);
		}
	}

	protected void RefreshPreviews()
	{
		ref array<ref EditorAssetsBrowserCard> cards = {};
		foreach (EditorAssetsBrowserSectionView section_view: m_SectionViews) {
			if (section_view) {
				section_view.AppendCards(cards);
			}
		}

		m_PreviewPool.Refresh(AssetsBrowserScroll, cards);
		m_PreviewRefreshTimer = 0.15;
		m_LastScrollPosition = AssetsBrowserScroll.GetVScrollPos();
	}

	void ToggleSectionCollapsed(string section_key)
	{
		if (section_key == string.Empty) {
			return;
		}

		if (m_CollapsedSections.Contains(section_key)) {
			m_CollapsedSections.Remove(section_key);
		} else {
			m_CollapsedSections.Set(section_key, true);
		}

		QueueRender(false, false, false);
	}

	protected void SyncToCurrentPlaceable()
	{
		EditorPlaceableItem current_placeable = GetCurrentPlaceable();
		if (!current_placeable) {
			return;
		}

		EditorAssetsBrowserEntry entry = m_Catalog.FindByPlaceable(current_placeable);
		if (!entry) {
			return;
		}

		m_FilterState.TabId = entry.TabId;
		m_FilterState.SubcategoryId = entry.SubcategoryId;
	}

	protected EditorPlaceableItem GetCurrentPlaceable()
	{
		if (m_Editor.IsPlacing()) {
			foreach (EditorWorldObject world_object: m_Editor.GetPlacingObjects()) {
				EditorHologram hologram;
				if (Class.CastTo(hologram, world_object)) {
					return hologram.GetPlaceableItem();
				}
			}
		}

		return m_Editor.GetObjectManager().CurrentSelectedItem;
	}

	protected string GetSectionStateKey(EditorAssetsBrowserSectionData section)
	{
		if (!section) {
			return string.Empty;
		}

		return m_FilterState.TabId + "|" + m_FilterState.SubcategoryId + "|" + section.Id;
	}

	protected bool IsSectionCollapsed(string section_key)
	{
		return m_CollapsedSections.Contains(section_key);
	}

	protected EditorAssetsBrowserFilterState CopyState()
	{
		EditorAssetsBrowserFilterState state = new EditorAssetsBrowserFilterState();
		state.TabId = m_FilterState.TabId;
		state.SubcategoryId = m_FilterState.SubcategoryId;
		state.PlatformFilter = m_FilterState.PlatformFilter;
		state.PlacementTypeFilter = m_FilterState.PlacementTypeFilter;
		state.FavoritesOnly = m_FilterState.FavoritesOnly;
		state.SelectedSourceIds = CopyStringArray(m_FilterState.SelectedSourceIds);
		state.SetSearchText(m_FilterState.SearchText);
		return state;
	}

	protected void SavePersistentState()
	{
		if (!IsPersistentStateChanged()) {
			return;
		}

		EditorSettings settings = m_Editor.GetSettings();
		settings.AssetsBrowserLastTabId = m_FilterState.TabId;
		settings.AssetsBrowserLastSubtabId = m_FilterState.SubcategoryId;
		settings.AssetsBrowserPlatformFilter = m_FilterState.PlatformFilter;
		settings.AssetsBrowserPlacementTypeFilter = m_FilterState.PlacementTypeFilter;
		settings.AssetsBrowserFavoritesOnly = m_FilterState.FavoritesOnly;
		settings.AssetsBrowserSelectedSources = CopyStringArray(m_FilterState.SelectedSourceIds);
		settings.Save();
		UpdateSavedStateSnapshot();
	}

	protected void QueuePersistentStateSave()
	{
		if (!IsPersistentStateChanged()) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(SavePersistentState);
			return;
		}

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(SavePersistentState);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SavePersistentState, PERSISTENT_STATE_SAVE_DELAY_MS, false);
	}

	protected bool IsPersistentStateChanged()
	{
		if (m_FilterState.TabId != m_SavedTabId) {
			return true;
		}

		if (m_FilterState.SubcategoryId != m_SavedSubcategoryId) {
			return true;
		}

		if (m_FilterState.PlatformFilter != m_SavedPlatformFilter) {
			return true;
		}

		if (m_FilterState.PlacementTypeFilter != m_SavedPlacementTypeFilter) {
			return true;
		}

		if (m_FilterState.FavoritesOnly != m_SavedFavoritesOnly) {
			return true;
		}

		if (!AreStringArraysEqual(m_FilterState.SelectedSourceIds, m_SavedSelectedSourceIds)) {
			return true;
		}

		return false;
	}

	protected void UpdateSavedStateSnapshot()
	{
		m_SavedTabId = m_FilterState.TabId;
		m_SavedSubcategoryId = m_FilterState.SubcategoryId;
		m_SavedPlatformFilter = m_FilterState.PlatformFilter;
		m_SavedPlacementTypeFilter = m_FilterState.PlacementTypeFilter;
		m_SavedFavoritesOnly = m_FilterState.FavoritesOnly;
		m_SavedSelectedSourceIds = CopyStringArray(m_FilterState.SelectedSourceIds);
	}

	protected void LayoutNavButtons(array<ref EditorAssetsBrowserNavButton> buttons, Widget container)
	{
		if (!container) {
			return;
		}

		int button_count = 0;
		for (int count_index = 0; count_index < buttons.Count(); count_index++) {
			if (buttons[count_index]) {
				button_count++;
			}
		}

		if (button_count == 0) {
			return;
		}

		float container_width;
		float container_height;
		container.GetScreenSize(container_width, container_height);
		if (container_width <= 0) {
			container.GetSize(container_width, container_height);
		}

		float total_spacing = FILTER_BUTTON_SPACING * (button_count - 1);
		float button_width = (container_width - total_spacing) / button_count;
		if (button_width < 1) {
			button_width = 1;
		}

		float x = 0;
		float row_height = 0;
		float y_offset = FILTER_ROW_VERTICAL_PADDING * 0.5;
		for (int i = 0; i < buttons.Count(); i++) {
			EditorAssetsBrowserNavButton button = buttons[i];
			if (!button) {
				continue;
			}

			button.SetWidth(button_width);
			button.GetLayoutRoot().SetPos(x, y_offset);
			x += button.GetWidth() + FILTER_BUTTON_SPACING;

			float padded_height = button.GetHeight() + FILTER_ROW_VERTICAL_PADDING;
			if (padded_height > row_height) {
				row_height = padded_height;
			}
		}

		float width, height;
		container.GetSize(width, height);
		if (row_height <= 0) {
			row_height = height;
		}

		container.SetSize(width, row_height);
	}

	protected float GetSectionContentWidth()
	{
		float content_width = 0;
		float height = 0;

		if (AssetsBrowserTabRow) {
			AssetsBrowserTabRow.GetScreenSize(content_width, height);
			if (content_width <= 0) {
				AssetsBrowserTabRow.GetSize(content_width, height);
			}
		}

		if (content_width <= 0 && AssetsBrowserSubcategoryRow) {
			AssetsBrowserSubcategoryRow.GetScreenSize(content_width, height);
			if (content_width <= 0) {
				AssetsBrowserSubcategoryRow.GetSize(content_width, height);
			}
		}

		if (content_width > 0) {
			return content_width;
		}

		float scroll_width, scroll_height;
		AssetsBrowserScroll.GetScreenSize(scroll_width, scroll_height);
		if (scroll_width <= 0) {
			AssetsBrowserScroll.GetSize(scroll_width, scroll_height);
		}

		scroll_width -= 24;
		return scroll_width;
	}

	protected bool IsDescendantOfAssetsBrowser(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserOverlay);
	}

	protected bool IsPartOfCurrentMenu(Widget widget)
	{
		if (!EditorHud.CurrentMenu) {
			return false;
		}

		return IsDescendantOfWidget(widget, EditorHud.CurrentMenu.GetLayoutRoot());
	}

	protected void CloseCurrentMenu()
	{
		delete EditorHud.CurrentMenu;
	}

	protected bool IsDescendantOfWidget(Widget widget, Widget parent_widget)
	{
		Widget current = widget;
		while (current) {
			if (current == parent_widget) {
				return true;
			}

			current = current.GetParent();
		}

		return false;
	}

	protected bool IsPartOfSourceFilterButton(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserSourceFilterRoot);
	}

	protected bool IsPartOfSourceFilterPopup(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserSourceFilterPopupRoot);
	}

	protected bool IsPartOfSourceFilterConsoleButton(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserSourceFilterConsoleRoot);
	}

	protected bool IsPartOfSourceFilterPCButton(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserSourceFilterPCRoot);
	}

	protected bool IsPartOfSourceFilterRevertButton(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserSourceFilterRevertRoot);
	}

	protected bool IsPartOfSourceFilterSaveButton(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserSourceFilterSaveRoot);
	}

	protected bool IsPartOfFavoritesOnlyButton(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserFavoritesOnlyRoot);
	}

	protected bool IsPartOfConfigOnlyButton(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserConfigOnlyRoot);
	}

	protected bool IsPartOfStaticOnlyButton(Widget widget)
	{
		return IsDescendantOfWidget(widget, AssetsBrowserStaticOnlyRoot);
	}

	protected void CreateTabButton(string id, string label)
	{
		EditorAssetsBrowserNavButton button = new EditorAssetsBrowserNavButton(this, label, EditorAssetsBrowserNavButtonType.EDITOR_ASSETS_BROWSER_NAV_TAB, id);
		button.SetSelected(id == m_FilterState.TabId);
		m_TabButtons.Insert(button);
		AssetsBrowserTabRow.AddChild(button.GetLayoutRoot());
	}

	protected void CreateSubcategoryButton(string id, string label)
	{
		EditorAssetsBrowserNavButton button = new EditorAssetsBrowserNavButton(this, label, EditorAssetsBrowserNavButtonType.EDITOR_ASSETS_BROWSER_NAV_SUBCATEGORY, id);
		button.SetSelected(id == m_FilterState.SubcategoryId);
		m_SubcategoryButtons.Insert(button);
		AssetsBrowserSubcategoryRow.AddChild(button.GetLayoutRoot());
	}



	protected void ClearTabButtons()
	{
		for (int i = m_TabButtons.Count() - 1; i >= 0; i--) {
			delete m_TabButtons[i];
		}

		m_TabButtons.Clear();
	}

	protected void ClearSubcategoryButtons()
	{
		for (int i = m_SubcategoryButtons.Count() - 1; i >= 0; i--) {
			delete m_SubcategoryButtons[i];
		}

		m_SubcategoryButtons.Clear();
	}

	protected void ClearSections()
	{
		for (int i = m_SectionViews.Count() - 1; i >= 0; i--) {
			delete m_SectionViews[i];
		}

		m_SectionViews.Clear();
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/gui/layouts/assetsbrowser/EditorAssetsBrowser.layout";
	}
}
