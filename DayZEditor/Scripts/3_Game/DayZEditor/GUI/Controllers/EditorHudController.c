class EditorFileHeadView: ScriptView
{	
	SpacerWidget TextSpacer;
	TextWidget Text;
	
	void EditorFileHeadView(string text)
	{
		Text.SetText(text);
		
		float w, h;
		TextSpacer.GetScreenSize(w, h);
		Print(w);
		
		Text.GetScreenSize(w, h);
		Print(w);
		
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\FileHead.layout";
	}
}

class EditorHudController: EditorControllerBase
{
	static const string NOTIFICATION_SOUND = "Notification_SoundSet";
	
	string Version = Editor.Version;
	
	string OnlineSessionId, OnlineEntityCount;
	int OnlineUserCount;
	
	StringEvaluater PrecisionLevel = "0.5";
	
	ref ObservableCollection<EditorTreeItem> LeftListItems = new ObservableCollection<EditorTreeItem>(this);
	ref ObservableCollection<EditorTreeItem> RightListItems = new ObservableCollection<EditorTreeItem>(this);
		
	ref ObservableCollection<ref EditorFileHeadView> OpenFiles = new ObservableCollection<ref EditorFileHeadView>(this);
	
	// Logger
	static const int MAX_LOG_ENTRIES = 20;
	ref ObservableCollection<ref EditorLogEntry> EditorLogEntries 			= new ObservableCollection<ref EditorLogEntry>(this);
	
	// Camera bindings
	float CameraSmoothing = 50.0;
	ref ObservableCollection<EditorCameraTrackListItem> CameraTrackData = new ObservableCollection<EditorCameraTrackListItem>(this);
	
	ScrollWidget LeftScroll;
	ScrollWidget RightScroll;
	
	protected Widget RightbarFrame;
	protected ImageWidget RightbarHideIcon;
	
	protected WrapSpacerWidget RightbarPlacementsList;
	protected WrapSpacerWidget RightbarDeletionsList;
	
	protected WrapSpacerWidget LeftbarPlacementsConfig, LeftbarPlacementsStatic;
	protected ButtonWidget LeftbarCategoryConfig, LeftbarCategoryStatic;
	
	protected GridSpacerWidget InfobarObjPosFrame;
		
	protected WrapSpacerWidget LeftbarPanelSelectorWrapper;
	protected EditBoxWidget LeftbarSearchBar;
	
	//
	protected EditBoxWidget PlacedSearchEditbox;
	
	protected ButtonWidget CinematicCameraButton;
	protected ButtonWidget PlacementsTabButton;
	protected ButtonWidget DeletionsTabButton;
	protected ButtonWidget LeftbarPanelSearchBarIconButton;
	protected ButtonWidget PlacedSearchIconButton;
	
	// Camera Track
	protected Widget CameraTrackWrapper;
	protected ButtonWidget CameraTrackRunButton;
	protected Widget CameraTrackButtonOutline;
	
	// Preview for Object Readout
	TextWidget ObjectHoverSelectObjectReadout;
	
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData = new ObservableCollection<ref EditorBrushData>(this);

	float BrushRadius = 65;
	float BrushDensity = 0.25;
	
	bool BrushToggleButtonState;
	int BrushTypeSelection;
	string BrushToggleButtonText;
	
	bool ControlPlayerState;
	
	protected ImageWidget MagnetButton_Icon;
	protected ImageWidget GroundButton_Icon;
	protected ImageWidget SnapButton_Icon;
	protected ImageWidget CollisionButton_Icon;
	protected ImageWidget CameraLightButton_Icon;
	
	ButtonWidget BrushToggleButton;
	TextWidget NotificationText;
	
	Widget NotificationPanel;
	
	// Favorites
	protected ref array<string> m_FavoriteItems = {};
		
	void EditorHudController() 
	{
		// more hacking
		g_EditorPrecision = GetPrecisionLevel();
	}
	
	void AddToHand(Object obj)
	{
		GetEditor().AddInHand(obj);
	}
		
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		// Reload Placeables
		array<string> favorite_items = {};
		GetGame().GetProfileStringList("EditorFavoriteItems", favorite_items);
		
		float widest_x;				
		// Load Brushes		
		string brush_file = m_Editor.GeneralSettings.EditorBrushFile;
		if (brush_file.Contains("'")) {
			// bi wtf
			brush_file.Replace("'", "");
			brush_file.Replace("\"", "");
			m_Editor.GeneralSettings.EditorBrushFile = brush_file;
			m_Editor.GeneralSettings.Save();
		}
		
		if (!FileExist(m_Editor.GeneralSettings.EditorBrushFile)) {
			if (!CopyFile("DayZEditor/scripts/data/Defaults/Brushes.xml", m_Editor.GeneralSettings.EditorBrushFile)) {
				EditorLog.Error("Could not copy brush data to %1", m_Editor.GeneralSettings.EditorBrushFile);
				return;
			}
		}
				
		ReloadBrushes(m_Editor.GeneralSettings.EditorBrushFile);
	}
		
	float GetPrecisionLevel()
	{
		return PrecisionLevel.Parse();
	}
		
	void CameraTrackToggleExecute(ButtonCommandArgs args) 
	{
		EditorLog.Trace("EditorHudController::CameraTrackToggleExecute");
		CameraTrackWrapper.Show(!CameraTrackWrapper.IsVisible());
	}

	void CameraTrackInsertNode(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorHudController::CameraTrackInsertNode");
		string name = "CameraTrack" + CameraTrackData.Count();
		GetEditor().GetCameraTrackManager().InsertCameraTrack(GetEditor().GetCamera(), 1.0, name);
	}

	void OnCameraTrackStart()
	{
		CameraTrackRunButton.SetText("Stop");
		CameraTrackRunButton.SetColor(COLOR_RED);
		CameraTrackButtonOutline.SetColor(COLOR_RED);
	}
	
	void OnCameraTrackStop()
	{
		CameraTrackRunButton.SetText("Start");
		CameraTrackRunButton.SetColor(COLOR_WHITE_A);
		CameraTrackButtonOutline.SetColor(COLOR_WHITE);
		CameraTrackRunButton.SetState(1);
	}
		
	void DoMultiSelect(int index_0, int index_1, array<EditorSelectableBase> list)
	{
		int bottom, top;
		bottom = Math.Min(index_0, index_1);
		top = Math.Max(index_0, index_1);
		
		if (list.Count() < top) {
			EditorLog.Error("Could not multi select, top was out of range of list array");
			return;
		}
		
		for (int i = bottom; i < top; i++) {			
			EditorPlacedListItem placed_list_item;
			
		}
	}
	
	// im not adding a trace to this lol
	void OnEditorLog(LogLevel level, string message)
	{
	}
	
	/*
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnMouseButtonDown");
		
		if (button == MouseState.RIGHT) {
		
			switch (w) {
				case LeftbarSearchBar: {
					SearchBarData = string.Empty;
					NotifyPropertyChanged("SearchBarData");
					break;
				}
				case PlacedSearchEditbox: {
					PlacedSearchBarData = string.Empty;
					NotifyPropertyChanged("PlacedSearchBarData");
					break;
				}
			}	
		}
	
		return super.OnMouseButtonDown(w, x, y, button);
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::OnMouseEnter");

		switch (w) {
			case PlacementsTabButton: {
				m_Editor.GetEditorHud().SetCurrentTooltip(EditorTooltip.CreateOnButton("" + GetEditor().GetPlacedObjects().Count() + " #STR_EDITOR_PLACEMENTS", w, TooltipPositions.BOTTOM_LEFT));
				break;
			}
			
			case DeletionsTabButton: {
				m_Editor.GetEditorHud().SetCurrentTooltip(EditorTooltip.CreateOnButton("" + GetEditor().GetDeletedObjects().Count() + " #STR_EDITOR_DELETIONS", w, TooltipPositions.BOTTOM_LEFT));
				break;
			}
			
			case CinematicCameraButton: {
				m_Editor.GetEditorHud().SetCurrentTooltip(EditorTooltip.CreateOnButton("#STR_EDITOR_CINEMATIC_CAMERA", w, TooltipPositions.TOP_LEFT));
				break;
			}
		}
				
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::OnMouseLeave");
		m_Editor.GetEditorHud().SetCurrentTooltip(null);
		
		return super.OnMouseLeave(w, enterW, x, y);
	}*/
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (RecursiveGetParent(w, ScrollWidget)) {
			if (KeyState(KeyCode.KC_LCONTROL)) {
				ScrollWidget.Cast(w).VScrollStep(wheel * 10);
			}
		}
		
		return false;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnFocus");
		
		switch (w) {
			
			case LeftbarSearchBar: {
				m_Editor.GetCamera().MoveEnabled = false;
				break;
			}
			case PlacedSearchEditbox:
				m_Editor.GetCamera().MoveEnabled = false;
				break;
		}
		
		return false;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnFocusLost");
		
		switch (w) {
			
			case LeftbarSearchBar: {
				m_Editor.GetCamera().MoveEnabled = true;
				break;
			}
			case PlacedSearchEditbox:
				m_Editor.GetCamera().MoveEnabled = true;
				break;
		}
		
		return false;
	}
		
	void ShowNotification(string text, int color, float duration)
	{
		NotificationPanel.SetColor(color);
		NotificationText.SetText(text);
		
		WidgetAnimator.Animate(NotificationPanel, WidgetAnimatorProperty.POSITION_Y, 90, 100);
		SEffectManager.PlaySoundOnObject(NOTIFICATION_SOUND, GetEditor().GetCamera());
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationPanel.SetPos, (duration * 1000) + 100, false, 0, 70, true);
	}
	
	// Brush Management
	void ReloadBrushes(string filename)
	{
		BrushToggleButtonState = false;
		NotifyPropertyChanged("BrushToggleButtonState");
		
		BrushTypeBoxData.Clear();
		XMLEditorBrushes xml_brushes = new XMLEditorBrushes(BrushTypeBoxData);
		GetXMLApi().Read(filename, xml_brushes);
	}
}