class EditorHudController: EditorControllerBase
{
	// Data Binding
	string DebugText1;
	string DebugText2;
	string DebugText3;
	string DebugText4;
	string DebugText5;
	string DebugText6;
	
	string SearchBarData;
	
	int PlaceableCategorySelection = 0;

	bool BuildingSelectData = true;
	bool VehicleSelectData;
	bool EntitySelectData;
	bool HumanSelectData;
	
	float cam_x, cam_y, cam_z;	
	float obj_x, obj_y, obj_z;
	
	float CameraTrackSmoothing;
	
	ref EditorHudToolbar EditorHudToolbarView;
	
	ref ObservableCollection<ref EditorPlaceableListItem> LeftbarSpacerData = new ObservableCollection<ref EditorPlaceableListItem>(this);
	ref ObservableCollection<ref EditorListItem> RightbarSpacerData 		= new ObservableCollection<ref EditorListItem>(this);
	
	ref ObservableCollection<ref EditorCameraTrackListItem> CameraTrackData = new ObservableCollection<ref EditorCameraTrackListItem>(this);
	
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
	
	protected Widget CameraTrackWrapper;
	
	// Temp until sub controllers can be properties of parent controller
	EditorHudToolbarController GetToolbarController() 
	{
		return EditorHudToolbarController.Cast(EditorHudToolbarView.GetController());
	}
	
	void EditorHudController() 
	{
		EditorLog.Trace("EditorHudController");
		EditorHud.CurrentEditorHudController = this;
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(Update);
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
#endif
	}
	
	void ~EditorHudController() 
	{
		EditorLog.Trace("~EditorHudController");
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Remove(Update);
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnObjectSelected.Remove(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Remove(OnObjectDeselected);
#endif
	}
	
	void Update()
	{
		Debug.DestroyAllShapes();
		array<EditorCameraTrackListItem> dta = {};
		for (int i = 0; i < CameraTrackData.Count(); i++) {
			if (CameraTrackData[i]) {
				dta.Insert(CameraTrackData[i]);
			}
		}		
		
		for (int k = 0; k < dta.Count(); k++) {

			EditorCameraTrackListItemController ctrl = dta[k].GetData();
			if (!dta[k + 1]) {
				continue;
			}
			
			EditorCameraTrackListItemController next_ctrl = dta[k + 1].GetData();
			
			float j;
			vector point = ctrl.GetPosition();
			vector old_point;
			while (j <= 1.0) {
				
				old_point = point;
				vector center_point = AverageVectors(ctrl.GetPosition(), next_ctrl.GetPosition()) + vector.Up * ((CameraTrackSmoothing / 100) * vector.Distance(ctrl.GetPosition(), next_ctrl.GetPosition()));
				
				point = EditorMath.CalculateQuadraticBezierPoint(j + 0.1, ctrl.GetPosition(), center_point, next_ctrl.GetPosition());
				
				Debug.DrawLine(old_point, point, COLOR_WHITE, ShapeFlags.NOZBUFFER);

				j += 0.1;
			}
			
			j = 0;
		}
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		// Reload Placeables
		EditorLog.Info("Loaded %1 Placeable Objects", ReloadPlaceableObjects().ToString());
		
		EditorHudToolbarView = new EditorHudToolbar();
		NotifyPropertyChanged("EditorHudToolbarView");
	}
			
	void InsertMapMarker(EditorMarker map_marker)
	{
		EditorLog.Trace("EditorHudController::InsertMapObject " + map_marker.GetLayoutRoot().GetName());
		m_Editor.GetEditorHud().EditorMapWidget.AddChild(map_marker.GetLayoutRoot());
	}
	
	int ReloadPlaceableObjects() 
	{ 
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

#ifdef COMPONENT_SYSTEM
				return i;
#endif
		    }
		}
		return i;
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
			
			case "BuildingSelectData": 
			case "VehicleSelectData": 
			case "HumanSelectData": 
			case "EntitySelectData": {
				/*
				TStringArray select_data = {"BuildingSelectData", "VehicleSelectData", "EntitySelectData", "HumanSelectData"};
				
				// Radio Button esque
				foreach (string data: select_data) {
					if (data != property_name) {
						bool result;
						EnScript.GetClassVar(this, data, 0, result);
						if (result) {
							EnScript.SetClassVar(this, data, 0, false);
							NotifyPropertyChanged(data, false);
						}
					}
				}
				
				for (int i = 0; i < LeftbarSpacerData.Count(); i++) {
					EditorPlaceableListItem list_item = LeftbarSpacerData[i];
					if (list_item) {
						list_item.GetLayoutRoot().Show(list_item.GetCategory() == select_data.Find(property_name));
					}
				}
				*/
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
		InsertCameraTrack(GetEditor().GetCamera(), 1.0, name);
	}
	
	void CameraTrackDeleteNode(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorHudController::CameraTrackDeleteNode");
	}
	
	void CameraTrackRunExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorHudController::CameraTrackRunExecute");
		
		thread _RunCameraTrack();
	}
	
	private void _RunCameraTrack()
	{
		EditorCamera camera = GetEditor().GetCamera();
		for (int i = 0; i < CameraTrackData.Count(); i++) {
			EditorCameraTrackListItemController ctrl = CameraTrackData[i].GetData();
			if (!CameraTrackData[i + 1]) {
				continue;
			}
			
			EditorCameraTrackListItemController next_pos = CameraTrackData[i + 1].GetData();
			vector start_position = ctrl.GetPosition();
			vector start_orientation = ctrl.GetOrientation();
			camera.SetPosition(start_position);
			camera.SetOrientation(start_orientation);
			
			int td = 0;
			while (td < ctrl.Time * 1000) {
				float time_value = 1 / (ctrl.Time * 1000) * td;
				vector position;
				position[0] = EditorMath.SmoothLerp(start_position[0], next_pos.pX.Parse(), time_value);
				position[1] = EditorMath.SmoothLerp(start_position[1], next_pos.pY.Parse(), time_value);
				position[2] = EditorMath.SmoothLerp(start_position[2], next_pos.pZ.Parse(), time_value);
				camera.SetPosition(position);
				
				vector orientation;
				orientation[0] = EditorMath.SmoothLerp(start_orientation[0], next_pos.oX.Parse(), time_value);
				orientation[1] = EditorMath.SmoothLerp(start_orientation[1], next_pos.oY.Parse(), time_value);
				orientation[2] = EditorMath.SmoothLerp(start_orientation[2], next_pos.oZ.Parse(), time_value);
				camera.SetOrientation(orientation);
				
				td += 10;
				Sleep(10);
			}
		}
	}
	
	void InsertCameraTrack(EditorCamera current_camera, float time, string name)
	{
		InsertCameraTrack(new EditorCameraTrackListItem(current_camera.GetPosition(), current_camera.GetOrientation(), time, name));
	}
	
	void InsertCameraTrack(EditorCameraTrackListItem camera_track_item)
	{
		CameraTrackData.Insert(camera_track_item);
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