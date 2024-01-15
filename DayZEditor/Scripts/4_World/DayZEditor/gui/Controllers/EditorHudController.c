class EditorHudController: EditorControllerBase
{
	static const string NOTIFICATION_SOUND = "Notification_SoundSet";
	
	string Version = Editor.Version;
	
	//
	string PlacedSearchBarData;
	string PlacedSearchBarIcon = "set:dayz_editor_gui image:search";
		
	bool CategoryPlacements = true;
	bool CategoryDeletions;
	bool CategoryConfig = true;
	bool CategoryStatic;
	bool FavoritesToggle;
	
	StringEvaluater PrecisionLevel = "0.5";
	
	float cam_x, cam_y, cam_z;	
	float obj_x, obj_y, obj_z;
		
	ref ObservableCollection<ref EditorTreeItem> LeftListItems = new ObservableCollection<ref EditorTreeItem>(this);
	ref ObservableCollection<ref EditorTreeItem> RightListItems = new ObservableCollection<ref EditorTreeItem>(this);
	
	ref EditorTreeItem PlacementsFolder, DeletionsFolder;
	
	ref ObservableCollection<EditorListItem> RightbarPlacedData 		= new ObservableCollection<EditorListItem>(this);
	ref ObservableCollection<EditorListItem> RightbarDeletionData 		= new ObservableCollection<EditorListItem>(this);
	
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
		
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		// Reload Placeables
		array<string> favorite_items = {};
		GetGame().GetProfileStringList("EditorFavoriteItems", favorite_items);
		
		float widest_x;
		map<int, ref array<EditorPlaceableItem>> all_placeable_items = GetEditor().GetPlaceableItemsByCategory();
		foreach (EditorPlaceableItemCategory category, array<EditorPlaceableItem> placeable_items: all_placeable_items) {
			EditorTreeItem tree_item = new EditorFolderTreeItem(category.GetDisplayName());
			
			foreach (EditorPlaceableItem placeable_item: placeable_items) {
				tree_item.GetTemplateController().Children.Insert(new EditorItemTreeItem(placeable_item));
			}
			
			LeftListItems.Insert(tree_item);
		}
				
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
		
		PlacementsFolder = new EditorFolderTreeItem("Placed Objects");
		DeletionsFolder = new EditorFolderTreeItem("Deleted Objects");
		RightListItems.Insert(PlacementsFolder);
		RightListItems.Insert(DeletionsFolder);
		
		ReloadBrushes(m_Editor.GeneralSettings.EditorBrushFile);
	}
				
	void InsertMapMarker(EditorMarker map_marker)
	{
		EditorLog.Trace("EditorHudController::InsertMapObject " + map_marker.GetLayoutRoot().GetName());
		m_Editor.GetEditorHud().EditorMapWidget.AddChild(map_marker.GetLayoutRoot());
	}
	
	float GetPrecisionLevel()
	{
		return PrecisionLevel.Parse();
	}
	
	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorHudController::PropertyChanged: %1", property_name);
		
		switch (property_name) {
					
			case "SearchBarData": {
				/*
				auto spacer_config = Ternary<ObservableCollection<ref EditorPlaceableListItem>>.If(CategoryConfig, LeftbarSpacerConfig, LeftbarSpacerStatic);
				for (int j = 0; j < spacer_config.Count(); j++) {
					if (FavoritesToggle) {
						spacer_config[j].GetLayoutRoot().Show(spacer_config[j].GetTemplateController().Favorite && spacer_config[j].FilterType(SearchBarData)); 	
					} else {
						spacer_config[j].GetLayoutRoot().Show(spacer_config[j].FilterType(SearchBarData)); 	
					}
				}
				
				LeftbarScroll.VScrollToPos(0);
				
				if (SearchBarData.Length() > 0) {
					SearchBarIcon = "set:dayz_gui image:icon_x";
				} else {
					SearchBarIcon = "set:dayz_editor_gui image:search";
				}
				
				NotifyPropertyChanged("SearchBarIcon");*/
				
				break;
			}	
			
			case "PlacedSearchBarData": {
				
				ObservableCollection<EditorListItem> selected_list;
				if (CategoryPlacements) {
					selected_list = RightbarPlacedData;
				} else {
					selected_list = RightbarDeletionData;
				}
					
				for (int k = 0; k < selected_list.Count(); k++) {
					selected_list[k].GetLayoutRoot().Show(selected_list[k].FilterType(PlacedSearchBarData)); 	
				}
				
				RightScroll.VScrollToPos(0);
				
				if (PlacedSearchBarData.Length() > 0) {
					PlacedSearchBarIcon = "set:dayz_gui image:icon_x";
				} else {
					PlacedSearchBarIcon = "set:dayz_editor_gui image:search";
				}
				
				NotifyPropertyChanged("PlacedSearchBarIcon");
				
				break;
			}
			
			case "FavoritesToggle": {
				/*
				auto spacer_config_favorites = Ternary<ObservableCollection<ref EditorPlaceableListItem>>.If(CategoryConfig, LeftbarSpacerConfig, LeftbarSpacerStatic);
				for (int i = 0; i < spacer_config_favorites.Count(); i++) {
					if (FavoritesToggle) {
						spacer_config_favorites[i].GetLayoutRoot().Show(spacer_config_favorites[i].GetTemplateController().Favorite && spacer_config_favorites[i].FilterType(SearchBarData));
					} else {
						spacer_config_favorites[i].GetLayoutRoot().Show(spacer_config_favorites[i].FilterType(SearchBarData)); 	 // SearchBarData == string.Empty || LeftbarSpacerData[i].FilterType(SearchBarData)
					}
				}
				
				LeftbarScroll.VScrollToPos(0);
				*/
				break;
			}		
			
			case "cam_x":
			case "cam_y":
			case "cam_z": {				
				GetEditor().GetCamera().SetPosition(Vector(cam_x, cam_y, cam_z));
				break;
			}
			
			case "CategoryPlacements": {
				CategoryDeletions = false;
				NotifyPropertyChanged("CategoryDeletions", false);
				// forcing to be true, otherwise it will just show nothing
				CategoryPlacements = true;
				
				RightbarPlacementsList.Show(CategoryPlacements);
				RightbarDeletionsList.Show(CategoryDeletions);
				
				PlacementsTabButton.SetColor(m_Editor.GeneralSettings.SelectionColor);
				DeletionsTabButton.SetColor(ARGB(255, 60, 60, 60));
				RightScroll.VScrollToPos(0);
				break;
			}
			
			case "CategoryDeletions": {				
				CategoryPlacements = false;				
				NotifyPropertyChanged("CategoryPlacements", false);				
				// forcing to be true, otherwise it will just show nothing
				CategoryDeletions = true;
				RightbarPlacementsList.Show(CategoryPlacements);
				RightbarDeletionsList.Show(CategoryDeletions);
				
				PlacementsTabButton.SetColor(ARGB(255, 60, 60, 60));
				DeletionsTabButton.SetColor(m_Editor.GeneralSettings.SelectionColor);
				RightScroll.VScrollToPos(0);
				break;
			}			
			
			case "CategoryConfig": {
				CategoryStatic = false;
				NotifyPropertyChanged("CategoryStatic", false);
				// forcing to be true, otherwise it will just show nothing
				CategoryConfig = true;
				
				LeftbarPlacementsConfig.Show(CategoryConfig);
				LeftbarPlacementsStatic.Show(CategoryStatic);
				
				LeftbarCategoryConfig.SetColor(m_Editor.GeneralSettings.SelectionColor);
				LeftbarCategoryStatic.SetColor(ARGB(255, 60, 60, 60));
				LeftScroll.VScrollToPos(0);
				break;
			}
			
			case "CategoryStatic": {				
				CategoryConfig = false;				
				NotifyPropertyChanged("CategoryPlacements", false);				
				// forcing to be true, otherwise it will just show nothing
				CategoryDeletions = true;
				LeftbarPlacementsConfig.Show(CategoryConfig);
				LeftbarPlacementsStatic.Show(CategoryStatic);
				
				LeftbarCategoryConfig.SetColor(ARGB(255, 60, 60, 60));
				LeftbarCategoryStatic.SetColor(m_Editor.GeneralSettings.SelectionColor);
				LeftScroll.VScrollToPos(0);
				break;
			}
						
			case "PlacedSearchBarIcon": {
				PlacedSearchIconButton.Enable(PlacedSearchBarData.Length() > 0);
				break;
			}
			
			// I literally hate this
			case "PrecisionLevel": {
				g_EditorPrecision = GetPrecisionLevel();
				break;
			}
			
			case "BrushToggleButtonState":
			case "BrushTypeSelection": {
				
				if (BrushTypeSelection < BrushTypeBoxData.Count()) {
					BrushToggleButtonText = BrushTypeBoxData[BrushTypeSelection].Type;
					NotifyPropertyChanged("BrushToggleButtonText", false);
				}
								
				m_Editor.CommandManager[EditorBrushToggleCommand].Execute(this, new ButtonCommandArgs(BrushToggleButton, 0));

				break;
			}
			
			case "BrushRadius":
			case "BrushDensity": {
				EditorBrush.BrushRadius = BrushRadius / 2;
				EditorBrush.BrushDensity = BrushDensity;
				break;
			}
			
			case "m_Editor.MagnetMode": {
				
				if (m_Editor.MagnetMode) {
					MagnetButton_Icon.SetColor(COLOR_CANDY);
				} else {
					MagnetButton_Icon.SetColor(COLOR_WHITE);
				}
				break;
			}
			case "m_Editor.GroundMode": {
				if (m_Editor.GroundMode) {
					GroundButton_Icon.SetColor(COLOR_APPLE);
				} else {
					GroundButton_Icon.SetColor(COLOR_WHITE);
				}
				
				break;
			}
			case "m_Editor.SnappingMode": {
				if (m_Editor.SnappingMode) {
					SnapButton_Icon.SetColor(COLOR_JELLY);
				} else {
					SnapButton_Icon.SetColor(COLOR_WHITE);
				}
				
				break;
			}
			case "m_Editor.CollisionMode": {
				if (m_Editor.CollisionMode) {
					CollisionButton_Icon.SetColor(COLOR_PALE_B);
				} else {
					CollisionButton_Icon.SetColor(COLOR_WHITE);
				}
					
				break;
			}
			case "ControlPlayerState": {
				if (ControlPlayerState) {
					GetEditor().ControlPlayer(PlayerBase.Cast(GetGame().GetPlayer()));
				} else {
					GetEditor().ControlCamera();
				}
				
				break;
			}
			
			case "m_Editor.CameraLight": {
				GetEditor().GetCamera().SetLightState(m_Editor.CameraLight);
				if (m_Editor.CameraLight) {
					CameraLightButton_Icon.SetColor(COLOR_YELLOW);
				} else {
					CameraLightButton_Icon.SetColor(COLOR_WHITE);
				}
				break;
			}
		}
	}
	
	void RightbarHideExecute(ButtonCommandArgs args) 
	{
		RightbarFrame.Show(!args.GetButtonState());
				
		if (args.GetButtonState()) {
			RightbarHideIcon.SetFlags(WidgetFlags.FLIPU);
		} else {
			RightbarHideIcon.ClearFlags(WidgetFlags.FLIPU);
		}
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

	void OnSearchPlacedButtonPress(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorHudController::OnSearchPlacedButtonPress");
		if (PlacedSearchBarData.Length() > 0) {
			PlacedSearchBarData = string.Empty;
			NotifyPropertyChanged("PlacedSearchBarData");
		}	
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
		
	void DoMultiSelect(int index_0, int index_1, ObservableCollection<EditorListItem> list)
	{
		int bottom, top;
		bottom = Math.Min(index_0, index_1);
		top = Math.Max(index_0, index_1);
		
		if (list.Count() < top) {
			EditorLog.Error("Could not multi select, top was out of range of list array");
			return;
		}
		
		for (int i = bottom; i < top; i++) {
			// if this element is filtered out
			if (!list[i].FilterType(PlacedSearchBarData)) {
				continue;
			}
			
			EditorPlacedListItem placed_list_item;
			// this is bad and wont work.. well it will but i dont like it
			if (Class.CastTo(placed_list_item, list[i])) {				
				placed_list_item.GetEditorObject().SetSelected(true);
			}
			
			EditorDeletedListItem deleted_list_item;
			if (Class.CastTo(deleted_list_item, list[i])) {
				deleted_list_item.GetDeletedObject().SetSelected(true);
			}
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
			
	void SetInfoObjectPosition(vector position)
	{
		obj_x = position[0];
		obj_y = position[1];
		obj_z = position[2];
		NotifyPropertyChanged("obj_x");
		NotifyPropertyChanged("obj_y");
		NotifyPropertyChanged("obj_z");
	}
	
	void ShowNotification(string text, int color, float duration)
	{
		NotificationPanel.SetColor(color);
		NotificationText.SetText(text);
		
		WidgetAnimator.Animate(NotificationPanel, WidgetAnimatorProperty.POSITION_Y, -25, 100);
		SEffectManager.PlaySoundOnObject(NOTIFICATION_SOUND, GetEditor().GetCamera());
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationPanel.SetPos, (duration * 1000) + 100, false, 0, 0, true);
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