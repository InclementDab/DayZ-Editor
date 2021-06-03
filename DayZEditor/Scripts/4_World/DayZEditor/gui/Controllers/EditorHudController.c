class EditorHudController: EditorControllerBase
{
	string SearchBarData;
	string Version = Editor.Version;
	
	int PlaceableCategorySelection = 0;
	
	float cam_x, cam_y, cam_z;	
	float obj_x, obj_y, obj_z;
	
	// Main data
	ref EditorHudToolbar EditorHudToolbarView;
	
	ref ObservableCollection<ref EditorPlaceableListItem> LeftbarSpacerData = new ObservableCollection<ref EditorPlaceableListItem>(this);
	ref ObservableCollection<ref EditorListItem> RightbarSpacerData 		= new ObservableCollection<ref EditorListItem>(this);
	
	// Logger
	LogLevel CurrentLogLevel = LogLevel.DEBUG;
	static const int MAX_LOG_ENTRIES = 15;
	ref ObservableCollection<ref EditorLogEntry> EditorLogEntries 			= new ObservableCollection<ref EditorLogEntry>(this);
	
	// Camera bindings
	float CameraSmoothing = 50.0;
	ref ObservableCollection<EditorCameraTrackListItem> CameraTrackData = new ObservableCollection<EditorCameraTrackListItem>(this);

	// View Properties
	protected Widget LeftbarFrame;
	protected ImageWidget LeftbarHideIcon;
	protected ScrollWidget LeftbarScroll;
	
	protected Widget RightbarFrame;
	protected ImageWidget RightbarHideIcon;
	protected ButtonWidget BrushToggleButton;
		
	protected GridSpacerWidget InfobarObjPosFrame;
		
	protected WrapSpacerWidget LeftbarPanelSelectorWrapper;
	protected EditBoxWidget LeftbarSearchBar;
	
	// Camera Track
	protected Widget CameraTrackWrapper;
	protected ButtonWidget CameraTrackRunButton;
	protected Widget CameraTrackButtonOutline;
	
	// Temp until sub controllers can be properties of parent controller
	EditorHudToolbarController GetToolbarController() 
	{
		return EditorHudToolbarController.Cast(EditorHudToolbarView.GetController());
	}
	
	void EditorHudController() 
	{
		EditorLog.Trace("EditorHudController");
		EditorHud.CurrentEditorHudController = this;
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
		
		EditorLog.OnLog.Insert(OnEditorLog);
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
#endif
	}
	
	void ~EditorHudController() 
	{
		EditorLog.Trace("~EditorHudController");
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnObjectSelected.Remove(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Remove(OnObjectDeselected);
		
		EditorLog.OnLog.Remove(OnEditorLog);
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
#endif
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		// Reload Placeables
#ifndef COMPONENT_SYSTEM
		EditorLog.Info("Loaded %1 Placeable Objects", ReloadPlaceableObjects().ToString());
#endif
		
		EditorHudToolbarView = new EditorHudToolbar();
		NotifyPropertyChanged("EditorHudToolbarView");
	}
		
	void Update()
	{
		Debug.DestroyAllShapes();
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
		m_Editor.GetEditorHud().EditorMapWidget.AddChild(map_marker.GetLayoutRoot());
	}
	
	int ReloadPlaceableObjects() 
	{ 
		g_Game.ReportProgress("Loading Placeable Objects");
		TStringArray config_paths = {};
		config_paths.Insert(CFG_VEHICLESPATH);
		config_paths.Insert(CFG_WEAPONSPATH);
		config_paths.Insert(CFG_MAGAZINESPATH);
		
		foreach (string path: config_paths) {
			
			for (int i = 0; i < GetWorkbenchGame().ConfigGetChildrenCount(path); i++) {
				string type;
		        GetWorkbenchGame().ConfigGetChildName(path, i, type);
				
				EditorPlaceableItem placeable_item = EditorPlaceableItem.Create(path, type);

				if (placeable_item && !IsForbiddenItem(placeable_item.Type)) {
					LeftbarSpacerData.Insert(new EditorPlaceableListItem(placeable_item));
				}
		    }
		}
		
		return LeftbarSpacerData.Count();
	}
	
	bool IsForbiddenItem(string Model)
	{
		//! In theory should be safe but just in case
		if (Model.Contains("Fx")) return true;
		if (Model == "ItemOptics") return true;

		//! Cursed items
		if (Model == "Red9") return true;
		if (Model == "QuickieBow") return true;
		if (Model == "LargeTentBackPack") return true;
		if (Model == "SurvivorMale_Base" || Model == "SurvivorFemale_Base") return true;
		if (GetGame().IsKindOf(Model, "GP25Base")) return true;
		if (GetGame().IsKindOf(Model, "M203Base")) return true;
	
		//! Everything is fine... I hope... :pain:
		return false;
	}

	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorHudController::PropertyChanged: %1", property_name);
		
		switch (property_name) {
					
			case "SearchBarData": {
				
				for (int j = 0; j < LeftbarSpacerData.Count(); j++) {
					EditorPlaceableListItem placeable_item = LeftbarSpacerData[j];
					placeable_item.GetLayoutRoot().Show(placeable_item.FilterType(SearchBarData)); 
				}
				
				LeftbarScroll.VScrollToPos(0);
				break;
			}			
			
			case "cam_x":
			case "cam_y":
			case "cam_z": {				
				GetEditor().GetCamera().SetPosition(Vector(cam_x, cam_y, cam_z));
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
		RightbarSpacerData.Insert(category);
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
			}	
		}
	
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	void DoMultiSelect(int index_0, int index_1, ObservableCollection<ref EditorListItem> list)
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
			if (Class.CastTo(placed_list_item, list[i])) {
				GetEditor().SelectObject(placed_list_item.GetEditorObject());
			}
		}
	}
	
	// im not adding a trace to this lol
	void OnEditorLog(LogLevel level, string message)
	{
		if (level < CurrentLogLevel) {
			return;
		}
		
		EditorLogEntries.Insert(new EditorLogEntry(level, message));
		if (EditorLogEntries.Count() > MAX_LOG_ENTRIES) {
			// todo: this is booming my mind for whatever reason
			//delete EditorLogEntries[1];
			//EditorLogEntries.Remove(1);
		}
	}
	
	// Raycast to see if TranslationWidget is under cursor	
	/*		
	RaycastRVParams raycast_params = new RaycastRVParams(GetGame().GetCurrentCameraPosition(), GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * EditorSettings.OBJECT_VIEW_DISTANCE);
	ref array<ref RaycastRVResult> raycast_result = new array<ref RaycastRVResult>();
	DayZPhysics.RaycastRVProxy(raycast_params, raycast_result);
	

	if (raycast_result.Count() > 0) {
		Object obj = raycast_result.Get(0).obj;
		if ((obj.GetType() == "TranslationWidget" || obj.GetType() == "RotationWidget")) {
			EditorEvents.DragInvoke(obj, m_Editor.GetTranslationWidget().GetEditorObject(), raycast_result.Get(0));
			return true;
		}
		
		EditorObject editor_object = m_Editor.GetObjectManager().GetEditorObject(obj);
		if (editor_object != null) {
			if (input.LocalValue("UAWalkRunTemp")) {
				EditorObjectManager.ToggleSelection(editor_object);
			} else if (!input.LocalValue("UATurbo")) {
				EditorEvents.ClearSelection(this);
			} else EditorEvents.SelectObject(this, editor_object);
			return true;
		}
	}
	*/
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (RecursiveGetParent(w, "LeftbarScroll") || RecursiveGetParent(w, "RightbarScroll")) {
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
		}
		
		return false;
	}
	
	private void OnObjectSelected(Class context, EditorObject target)
	{
		InfobarObjPosFrame.Show(m_Editor.GetObjectManager().GetSelectedObjects().Count() > 0);
	}
	
	private void OnObjectDeselected(Class context, EditorObject target)
	{
		InfobarObjPosFrame.Show(m_Editor.GetObjectManager().GetSelectedObjects().Count() > 0);
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
}