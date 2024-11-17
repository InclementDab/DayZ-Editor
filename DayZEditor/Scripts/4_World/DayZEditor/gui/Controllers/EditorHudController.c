class EditorHudController: EditorControllerBase
{
	string Version = Editor.Version;
	
	//	
	string ObjectReadoutName;
	
	bool CategoryPlacements = true;
	bool CategoryDeletions;
	bool CategoryConfig = true;
	bool CategoryStatic;
	bool FavoritesToggle;
	bool ShowPrivate;
	
	float cam_x, cam_y, cam_z;	
	float obj_x, obj_y, obj_z;
	
	// Main data	
	ref ObservableCollection<ref EditorPlaceableListItem> LeftbarSpacerConfig = new ObservableCollection<ref EditorPlaceableListItem>(this);
	ref ObservableCollection<ref EditorPlaceableListItem> LeftbarSpacerStatic = new ObservableCollection<ref EditorPlaceableListItem>(this);
	
	ref ObservableCollection<EditorListItem> RightbarPlacedData 		= new ObservableCollection<EditorListItem>(this);
	ref ObservableCollection<EditorListItem> RightbarDeletionData 		= new ObservableCollection<EditorListItem>(this);
	
	// Logger
	static const int MAX_LOG_ENTRIES = 20;
	ref ObservableCollection<ref EditorLogEntry> EditorLogEntries 			= new ObservableCollection<ref EditorLogEntry>(this);
	
	// Camera bindings
	float CameraSmoothing = 50.0;
	ref ObservableCollection<EditorCameraTrackListItem> CameraTrackData = new ObservableCollection<EditorCameraTrackListItem>(this);

	// View Properties
	Widget LeftbarFrame;
	protected ImageWidget LeftbarHideIcon;
	
	ScrollWidget LeftbarScroll;
	ScrollWidget RightbarScroll;
	EditBoxWidget LeftSearchBar, RightSearchBar;
	
	Widget RightbarFrame;
	protected ImageWidget RightbarHideIcon;
	
	protected WrapSpacerWidget RightbarPlacementsList;
	protected WrapSpacerWidget RightbarDeletionsList;
	
	protected WrapSpacerWidget LeftbarPlacementsConfig, LeftbarPlacementsStatic;
	
	protected GridSpacerWidget InfobarObjPosFrame;
		
	protected WrapSpacerWidget LeftbarPanelSelectorWrapper;
			
	// Camera Track
	protected Widget CameraTrackWrapper;

	protected ButtonWidget GizmoTranslateButton, FavoritesTabButton, ShowPrivateButton, LeftbarCategoryStatic, LeftbarCategoryConfig;
	protected ButtonWidget LeftbarHide, DeletionsTabButton, PlacementsTabButton, RightbarHide;
	protected ButtonWidget MenuBarFile, MenuBarEdit, MenuBarView, MenuBarEditor;
	protected ButtonWidget BrushToggleButton, CinematicCameraButton, CameraTrackMinimizeButton, AddNodeButton, CameraTrackRunButton;
	protected ButtonWidget ObjectSelectionButton, BoxSelectionButton, EllipseSelectionButton, LassoSelectionButton;
	
	protected Widget CameraTrackButtonOutline;
	
	// Preview for Object Readout
	TextWidget ObjectHoverSelectObjectReadout;
	
	// Favorites
	protected ref array<string> m_FavoriteItems = {};


	// Toolvar stuff
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData = new ObservableCollection<ref EditorBrushData>(this);

	float BrushRadius = 65;
	float BrushDensity = 0.25;
	float BrushWidth = 2.0;
	
	bool BrushToggleButtonState;
	int BrushTypeSelection;
	string BrushToggleButtonText;
				
	void EditorHudController() 
	{		
		m_Editor = GetEditor();
#ifndef COMPONENT_SYSTEM	

		EditorLog.OnLog.Insert(OnEditorLog);		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
#endif
	}
	
	void ~EditorHudController() 
	{		
#ifndef COMPONENT_SYSTEM		
		EditorLog.OnLog.Remove(OnEditorLog);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
#endif
	}
		
	void Update()
	{
		//Debug.DestroyAllShapes();
/*
		array<EditorCameraTrackListItem> camera_tracks = GetCameraTracks();
		for (int i = 0; i < camera_tracks.Count(); i++) {
			EditorCameraTrackListItemController start_ctrl = camera_tracks[i].GetData();
			if (!camera_tracks[i + 1]) {
				continue;
			}
			
			EditorCameraTrackListItemController end_ctrl = camera_tracks[i + 1].GetData();
			
			float value = 0;
			vector last_position = start_ctrl.GetPosition();
			while (value <= 1.0) {
				vector position = EditorMath.LerpVector(start_ctrl.GetPosition(), end_ctrl.GetPosition(), value);
				Debug.DrawLine(last_position, position, COLOR_WHITE);
				last_position = position;
				value += 0.05;
			}
		}*/
	}
			
	void InsertMapMarker(EditorMarker map_marker)
	{
		EditorLog.Trace("EditorHudController::InsertMapObject " + map_marker.GetLayoutRoot().GetName());
		m_Editor.GetEditorHud().Map.AddChild(map_marker.GetLayoutRoot());
	}
	
	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorHudController::PropertyChanged: %1", property_name);
		
		switch (property_name) {
			case "FavoritesToggle":
			case "ShowPrivate": {
				GetEditor().GetSettings().ShowFavoriteObjects = FavoritesToggle;
				GetEditor().GetSettings().ShowScopeZeroObjects = ShowPrivate;
				GetEditor().GetSettings().Save();

				auto spacer_config = Ternary<ObservableCollection<ref EditorPlaceableListItem>>.If(CategoryConfig, LeftbarSpacerConfig, LeftbarSpacerStatic);
				for (int j = 0; j < spacer_config.Count(); j++) {
					int hide = !spacer_config[j].FilterType(LeftSearchBar.GetText());
					if (FavoritesToggle) {
						hide |= hide | (!spacer_config[j].GetTemplateController().Favorite << 1);
					}

					if (!ShowPrivate) {
						hide |= hide | (spacer_config[j].GetPlaceableItem().Scope < 2) << 2;
					}

					spacer_config[j].GetLayoutRoot().Show(!hide);
				}

				break;
			}

			case "BrushToggleButtonState":
			case "BrushTypeSelection": {
				
				if (BrushTypeSelection < BrushTypeBoxData.Count()) {
					BrushToggleButtonText = BrushTypeBoxData[BrushTypeSelection].Name;
					NotifyPropertyChanged("BrushToggleButtonText", false);
				}
								
				m_Editor.CommandManager[EditorBrushToggleCommand].Execute(this, new ButtonCommandArgs(BrushToggleButton, 0));

				break;
			}
			
			case "BrushRadius":
			case "BrushDensity":
			case "BrushWidth": {
				EditorBrush.BrushRadius = BrushRadius / 2;
				EditorBrush.BrushDensity = BrushDensity;
				EditorBrush.BrushWidth = BrushWidth;
				break;
			}
		}

		switch (property_name) {
					
			case "SearchBarData": {
				/*
				LeftbarScroll.VScrollToPos(0);
				
				if (SearchBarData.Length() > 0) {
					SearchBarIcon = "set:dayz_gui image:icon_x";
				} else {
					SearchBarIcon = "set:dayz_editor_gui image:search";
				}
				
				NotifyPropertyChanged("SearchBarIcon");
				*/
				break;
			}	
			
			case "PlacedSearchBarData": {
				/*
				ObservableCollection<EditorListItem> selected_list;
				if (CategoryPlacements) {
					selected_list = RightbarPlacedData;
				} else {
					selected_list = RightbarDeletionData;
				}
					
				for (int k = 0; k < selected_list.Count(); k++) {
					selected_list[k].GetLayoutRoot().Show(selected_list[k].FilterType(PlacedSearchBarData)); 	
				}
				
				RightbarScroll.VScrollToPos(0);
				
				if (PlacedSearchBarData.Length() > 0) {
					PlacedSearchBarIcon = "set:dayz_gui image:icon_x";
				} else {
					PlacedSearchBarIcon = "set:dayz_editor_gui image:search";
				}
				
				NotifyPropertyChanged("PlacedSearchBarIcon");
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
				
				PlacementsTabButton.SetColor(m_Editor.GetSettings().SelectionColor);
				DeletionsTabButton.SetColor(ARGB(255, 60, 60, 60));
				RightbarScroll.VScrollToPos(0);
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
				DeletionsTabButton.SetColor(m_Editor.GetSettings().SelectionColor);
				RightbarScroll.VScrollToPos(0);
				break;
			}			
			
			case "CategoryConfig": {
				CategoryStatic = false;
				NotifyPropertyChanged("CategoryStatic", false);
				// forcing to be true, otherwise it will just show nothing
				CategoryConfig = true;
				
				LeftbarPlacementsConfig.Show(CategoryConfig);
				LeftbarPlacementsStatic.Show(CategoryStatic);
				
				LeftbarCategoryConfig.SetColor(m_Editor.GetSettings().SelectionColor);
				LeftbarCategoryStatic.SetColor(ARGB(255, 60, 60, 60));
				LeftbarScroll.VScrollToPos(0);
				break;
			}
			
			case "CategoryStatic": {				
				CategoryConfig = false;				
				NotifyPropertyChanged("CategoryPlacements", false);				
				// forcing to be true, otherwise it will just show nothing
				CategoryStatic = true;
				LeftbarPlacementsConfig.Show(CategoryConfig);
				LeftbarPlacementsStatic.Show(CategoryStatic);
				
				LeftbarCategoryConfig.SetColor(ARGB(255, 60, 60, 60));
				LeftbarCategoryStatic.SetColor(m_Editor.GetSettings().SelectionColor);
				LeftbarScroll.VScrollToPos(0);
				break;
			}
		}
	}
	
	void LeftbarHideExecute(ButtonCommandArgs args) 
	{
		LeftbarFrame.Show(!args.GetButtonState());
		
		if (args.GetButtonState()) {
			LeftbarHideIcon.SetFlags(WidgetFlags.FLIPU);
		} else {
			LeftbarHideIcon.ClearFlags(WidgetFlags.FLIPU);
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
		
	void ButtonCreateFolderExecute(ButtonCommandArgs args) 
	{
		EditorLog.Trace("EditorHudController::ButtonCreateFolderExecute");
		EditorCollapsibleListItem category(null);
		RightbarPlacedData.Insert(category);
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
			if (!list[i].FilterType(RightSearchBar.GetText())) {
				continue;
			}
			
			EditorPlacedListItem placed_list_item;
			// this is bad and wont work.. well it will but i dont like it
			if (Class.CastTo(placed_list_item, list[i])) {				
				GetEditor().SelectObject(placed_list_item.GetEditorObject());
			}
			
			EditorDeletedListItem deleted_list_item;
			if (Class.CastTo(deleted_list_item, list[i])) {
				GetEditor().SelectHiddenObject(deleted_list_item.GetDeletedObject());
			}
		}
	}
	
	// im not adding a trace to this lol
	void OnEditorLog(LogLevel level, string message)
	{
		if (!m_Editor || !m_Editor.GetSettings() || level < m_Editor.GetSettings().SelectedLogLevel) {
			return;
		}
		
		EditorLogEntries.Insert(new EditorLogEntry(level, message));
		if (EditorLogEntries.Count() > MAX_LOG_ENTRIES) {
			EditorLogEntries.RemoveOrdered(0); // 0 = remove oldest, then since ordered next olded becomes 0. i think????
		}
	}
			
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (EditorHud.CurrentDialog && !EditorHud.IsDialogCommand(w)) {
			return super.OnMouseEnter(w, x, y);
		}
		
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
		
		switch (w.GetTypeName()) {
			
			case "SliderWidget": {
				w.SetColor(GetEditor().GetSettings().SelectionColor);
				break;
			}
		}
		
		ViewBinding view_binding = GetViewBinding(w);
		if (view_binding && !EditorHud.CurrentMenu) {
			EditorCommand editor_command;
			if (Class.CastTo(editor_command, view_binding.GetRelayCommand())) {
				
				float pos_x, pos_y, size_x, size_y;
				w.GetScreenPos(pos_x, pos_y);
				w.GetScreenSize(size_x, size_y);
								
				EditorTooltip tooltip = EditorTooltip.CreateOnButton(editor_command, w, TooltipPositions.BOTTOM_LEFT);
				if (!editor_command.CanExecute()) {
					tooltip.GetLayoutRoot().SetAlpha(100);
				}
				
				m_Editor.GetEditorHud().SetCurrentTooltip(tooltip);
				
			}
		}
		
		switch (w) {
			
			case MenuBarFile:
			case MenuBarEdit:
			case MenuBarView:
			case MenuBarEditor: {
				
				if (EditorHud.CurrentMenu) {
					delete EditorHud.CurrentMenu;
					EditorHud.CurrentMenu = CreateToolbarMenu(w);
				}
				
				break;
			}	
			
		}
				
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_Editor.GetEditorHud().SetCurrentTooltip(null);

		SymbolHandler handler;

		switch (w.GetTypeName()) {
		
			case "SliderWidget": {
				w.SetColor(COLOR_WHITE_A);
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
		

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{		
		if (RecursiveGetParent(w, ScrollWidget)) {
			if (KeyState(KeyCode.KC_LCONTROL)) {
				ScrollWidget.Cast(w).VScrollStep(wheel * 10);
				return true;
			}
		}
		
		string w_name = w.GetName();
		float direction = wheel;
		switch (w_name) {
			case "BrushRadiusText":
			case "BrushRadiusSlider": {
				BrushRadius += direction * 2;
				BrushRadius = Math.Clamp(BrushRadius, 1, 100);
				NotifyPropertyChanged("BrushRadius");
				break;
			}
			
			case "BrushDensityText":
			case "BrushDensitySlider": {
				BrushDensity += direction * 0.05;
				BrushDensity = Math.Clamp(BrushDensity, 0, 1);
				NotifyPropertyChanged("BrushDensity");
				break;
			}			

			case "BrushWidthText": 
			case "BrushDensitrySlider": {
				BrushWidth += direction;
				BrushWidth = Math.Clamp(BrushWidth, 0, BrushRadius);
				NotifyPropertyChanged("BrushWidth");
				break;
			}
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

	// Relay Commands
	void MenuBarExecute(ButtonCommandArgs args) 
	{		
		EditorLog.Trace("EditorHudToolbarController::MenuBarExecute");
		if (!EditorHud.CurrentMenu) { //  GetMenu().Type() != GetBoundMenu(args.GetButtonWidget()) removed cause GetBoundMenu is gone
			EditorHud.CurrentMenu = CreateToolbarMenu(args.Source);
		} else {
			delete EditorHud.CurrentMenu;
		}
	}	
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		EditorLog.Trace("EditorHudToolbarController::CollectionChanged: " + collection_name);
		switch (collection_name) {
			
			case "BrushTypeBoxData": {
				
				m_Editor.CommandManager[EditorBrushToggleCommand].SetCanExecute(args.Source.Count() > 0);
				
				if (BrushTypeSelection < BrushTypeBoxData.Count()) {
					BrushToggleButtonText = BrushTypeBoxData[BrushTypeSelection].Name;
					NotifyPropertyChanged("BrushToggleButtonText", false);
				}
				break;
			}
		}
	}

	private EditorMenu CreateToolbarMenu(Widget toolbar_button)
	{
		EditorLog.Trace("EditorHudToolbarController::CreateToolbarMenu");	
				
		EditorMenu toolbar_menu;
		switch (toolbar_button) {
			
			case MenuBarFile: {
				toolbar_menu = new EditorFileMenu();
				break;
			}
			
			case MenuBarEdit: {
				toolbar_menu = new EditorEditMenu();
				break;
			}
			
			case MenuBarView: {
				toolbar_menu = new EditorViewMenu();
				break;
			}
			
			case MenuBarEditor: {
				toolbar_menu = new EditorEditorMenu(); // lol
				break;
			}
		}
		
		// Sets position to bottom of button
		float x, y, w, h;
		toolbar_button.GetScreenPos(x, y);
		toolbar_button.GetScreenSize(w, h);
		toolbar_menu.GetLayoutRoot().SetPos(x, y + h);
		
		return toolbar_menu;
	}
}