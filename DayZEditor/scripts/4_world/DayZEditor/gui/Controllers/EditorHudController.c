
class EditorUIManager
{
	// Modal Menu Control
	static ref EditorMenu CurrentMenu;
	
	// ToolTip Control
	static ref EditorTooltip CurrentTooltip;
		
	// Dialog Control
	static ref EditorDialog CurrentDialog;
	
	static 	bool IsDialogCommand(Widget w) {
		return (CurrentDialog && CurrentDialog.GetLayoutRoot() && CurrentDialog.GetLayoutRoot().FindAnyWidget(w.GetName()));
	}
	
	static ref EditorHudController CurrentEditorHudController;
	static ref EditorHudToolbarController CurrentEditorHudToolbarController;
}



class EditorHudController: Controller
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

	
	float cam_x, cam_y, cam_z;	
	float obj_x, obj_y, obj_z;
	
	ref ObservableCollection<ref EditorListItem> LeftbarSpacerData 		= new ObservableCollection<ref EditorListItem>("LeftbarSpacerData", this);
	ref ObservableCollection<ref EditorListItem> RightbarSpacerData 	= new ObservableCollection<ref EditorListItem>("RightbarSpacerData", this);
	ref ObservableCollection<string> DebugActionStackListbox 			= new ObservableCollection<string>("DebugActionStackListbox", this);
	
	// View Properties
	protected Widget LeftbarFrame;
	protected ImageWidget LeftbarHideIcon;
	protected ScrollWidget LeftbarScroll;
	
	protected Widget RightbarFrame;
	protected ImageWidget RightbarHideIcon;
	protected ButtonWidget BrushToggleButton;
		
	protected GridSpacerWidget InfobarObjPosFrame;
	
	CanvasWidget EditorCanvas;
		
	protected WrapSpacerWidget LeftbarPanelSelectorWrapper;
	//protected RadioButtonGroup m_RadioButtonGroup;

	
	protected EditBoxWidget LeftbarSearchBar;

	// Temp until sub controllers can be properties of parent controller
	EditorHudToolbarController GetToolbarController() {
		return EditorUIManager.CurrentEditorHudToolbarController;
	}
	
	void EditorHudController() 
	{
		EditorLog.Trace("EditorHudController");
		EditorUIManager.CurrentEditorHudController = this;
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
#endif
	}
	
	void ~EditorHudController() 
	{
		EditorLog.Trace("~EditorHudController");
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnObjectSelected.Remove(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Remove(OnObjectDeselected);
#endif
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);

		
		// Reload Placeables
		EditorLog.Info("Loaded %1 Placeable Objects", ReloadPlaceableObjects().ToString());
	}
			
	void InsertMapMarker(EditorMarker map_marker)
	{
		EditorLog.Trace("EditorHudController::InsertMapObject " + map_marker.GetLayoutRoot().GetName());
		GetEditor().GetEditorHud().EditorMapWidget.AddChild(map_marker.GetLayoutRoot());
	}
	
	int ReloadPlaceableObjects() 
	{ 
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);
		paths.Insert(CFG_WEAPONSPATH);
		paths.Insert(CFG_MAGAZINESPATH);
		
		foreach (string config_path: paths) {
			
			for (int j = 0; j < GetWorkbenchGame().ConfigGetChildrenCount(config_path); j++) {
				string class_name;
		        GetWorkbenchGame().ConfigGetChildName(config_path, j, class_name);
				TStringArray full_path = new TStringArray();
				GetWorkbenchGame().ConfigGetFullPath(config_path + " " + class_name, full_path);
				
				if (full_path.Find("HouseNoDestruct") != -1) {
					EditorPlaceableListItem item = new EditorPlaceableListItem(null, class_name, config_path);
					LeftbarSpacerData.Insert(item);
					
				} else {
					EditorLog.Trace("Not including Base " + full_path.ToString()); //find what isnt loading here
				}
#ifdef COMPONENT_SYSTEM
				if (j > 500) return 500;
#endif
		    }
		}
		return j;
	}
	

	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorHudController::PropertyChanged: " + property_name);
		
		switch (property_name) {
					
			case "SearchBarData": {
				
				for (int j = 0; j < LeftbarSpacerData.Count(); j++) {
					EditorPlaceableListItem placeable_item = LeftbarSpacerData.Get(j);
					placeable_item.GetLayoutRoot().Show(placeable_item.FilterType(SearchBarData));
				}
				
				LeftbarScroll.HScrollToPos(0);
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
		EditorCollapsibleListItem category();
		RightbarSpacerData.Insert(category);
	}	



	/*
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnMouseButtonDown");
		if (button == MouseState.LEFT) {
		

			
		// Raycast to see if TranslationWidget is under cursor	
			/*		
			RaycastRVParams raycast_params = new RaycastRVParams(GetGame().GetCurrentCameraPosition(), GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * EditorSettings.OBJECT_VIEW_DISTANCE);
			ref array<ref RaycastRVResult> raycast_result = new array<ref RaycastRVResult>();
			DayZPhysics.RaycastRVProxy(raycast_params, raycast_result);
			

			if (raycast_result.Count() > 0) {
				Object obj = raycast_result.Get(0).obj;
				if ((obj.GetType() == "TranslationWidget" || obj.GetType() == "RotationWidget")) {
					EditorEvents.DragInvoke(obj, GetEditor().GetTranslationWidget().GetEditorObject(), raycast_result.Get(0));
					return true;
				}
				
				EditorObject editor_object = GetEditor().GetObjectManager().GetEditorObject(obj);
				if (editor_object != null) {
					if (input.LocalValue("UAWalkRunTemp")) {
						EditorObjectManager.ToggleSelection(editor_object);
					} else if (!input.LocalValue("UATurbo")) {
						EditorEvents.ClearSelection(this);
					} else EditorEvents.SelectObject(this, editor_object);
					return true;
				}
			}
			
			
			
		}
		
		return false;
	}*/

	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnMouseButtonUp");
		
		if (button != 0) return false;

		switch (w.GetTypeName()) {

			case "ButtonWidget": {
				//w.SetColor(COLOR_EMPTY);
				//ButtonWidget.Cast(w).SetState(false);
				//SetWidgetIconPosition(w, 0, 0);
				break;
			}
		}
		
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnClick");
		
		switch (button) {
			
			case 0: {
		
				switch (w.GetName()) {
					
					case "UndoButton": 
					case "RedoButton": {
						w.SetColor(COLOR_SALMON_A);
						int pos = ButtonWidget.Cast(w).GetState() * 1;
						SetWidgetIconPosition(w, pos, pos);
						break;
					}
					
					case "SnapButton":
					case "GroundButton":
					case "MagnetButton": {
						bool button_state = ButtonWidget.Cast(w).GetState();
						GetWidgetIcon(w).SetColor((GetHighlightColor(w.GetName()) * button_state) - 1);
						GetWidgetIcon(w).SetPos(button_state * 1, button_state * 1);
						break;
					}
				}
							
				break;
			}
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	
	override bool OnFocus(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnFocus");
		
		switch (w) {
			
			case LeftbarSearchBar: {
				GetEditor().GetCamera().MoveEnabled = false;
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
				GetEditor().GetCamera().MoveEnabled = true;
				break;
			}
		}
		
		return false;
	}
	
		
	private int GetHighlightColor(string widget_name)
	{
		switch (widget_name) {
			
			case "UndoButton": 
			case "RedoButton": {
				return COLOR_SALMON_A;
			}
			
			case "MagnetButton": {
				return COLOR_CANDY;
			}
			
			case "GroundButton": {
				return COLOR_APPLE;
			}
			
			case "SnapButton": {
				return COLOR_JELLY;
			}
			
			default: {
				return COLOR_SALMON;
			}
		}
		
		return 0;
	}
	
	private ImageWidget GetWidgetIcon(Widget w)	{
		return ImageWidget.Cast(w.FindAnyWidget(string.Format("%1_Icon", w.GetName())));
	}
	
	private void SetWidgetIconPosition(Widget w, int x, int y) {
		Widget icon = GetWidgetIcon(w);
		if (icon) {
			icon.SetPos(x, y);
		}
	}
		
	ScriptInvoker DragBoxQueue = GetGame().GetUpdateQueue(CALL_CATEGORY_GUI);
	private int start_x, start_y;
	void DelayedDragBoxCheck()
	{
		GetMousePos(start_x, start_y);
		if (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)
			DragBoxQueue.Insert(_DragBoxUpdater);
	}
	
	private void _DragBoxUpdater()
	{
		int current_x, current_y;
		GetMousePos(current_x, current_y);
		EditorCanvas.Clear();
		int selection_box_thickness = 2;
		int selection_box_color = ARGB(100, 255, 0, 0);
		EditorCanvas.DrawLine(start_x, start_y, current_x, start_y, selection_box_thickness, selection_box_color);
		EditorCanvas.DrawLine(start_x, start_y, start_x, current_y, selection_box_thickness, selection_box_color);
		EditorCanvas.DrawLine(start_x, current_y, current_x, current_y, selection_box_thickness, selection_box_color);
		EditorCanvas.DrawLine(current_x, start_y, current_x, current_y, selection_box_thickness, selection_box_color);
		
		
		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
			EditorCanvas.Clear();
			DragBoxQueue.Remove(_DragBoxUpdater);
		}
			
		int x_low, x_high, y_low, y_high;
		if (start_x > current_x) {
			x_high = start_x;
			x_low = current_x;
		} else { 
			x_high = current_x;
			x_low = start_x;
		}
		
		if (start_y > current_y) {
			y_high = start_y;
			y_low = current_y;
		} else { 
			y_high = current_y;
			y_low = start_y;
		}
		
		ref EditorObjectSet placed_objects = GetEditor().GetPlacedObjects();
		foreach (EditorObject editor_object: placed_objects) {
			
			float marker_x, marker_y;
			// this kinda sucks
			if (GetEditor().GetEditorHud().EditorMapWidget.IsVisible()) {
				editor_object.GetMapMarkerPosition(marker_x, marker_y);
			} else {
				editor_object.GetObjectMarkerPosition(marker_x, marker_y);
			}
			
			
			if ((marker_x < x_high && marker_x > x_low) && (marker_y < y_high && marker_y > y_low)) {		
				if (!editor_object.IsSelected())
					GetEditor().SelectObject(editor_object);
			}
		}		
	}
		/*
	
	private void OnRadioButtonActivate(RadioButton radio_button)
	{
		EditorLog.Trace("EditorHudController::OnRadioButtonActivate");
		Widget root = radio_button.GetRoot();
		root.SetColor(COLOR_SALMON_A);
		int pos = ButtonWidget.Cast(root).GetState() * 1;
		SetWidgetIconPosition(root, pos, pos);

		// if you add more radio buttons. put this in a condition :)
		for (int i = 0; i < LeftbarSpacerData.Count(); i++) {
			Widget list_item = LeftbarSpacerData[i].GetLayoutRoot();
			EditorPlaceableListItem item;
			list_item.GetUserData(item);
			if (list_item && item) {
				list_item.Show(item.GetCategory() == radio_button.GetID());
			}
		}
	}
	
	private void OnRadioButtonDeactivate(RadioButton radio_button)
	{
		EditorLog.Trace("EditorHudController::OnRadioButtonDeactivate");
		Widget root = radio_button.GetRoot();
		root.SetColor(COLOR_EMPTY);
		SetWidgetIconPosition(root, 0, 0);
	}
		*/
	private void OnObjectSelected(Class context, EditorObject target)
	{
		InfobarObjPosFrame.Show(GetEditor().GetObjectManager().GetSelectedObjects().Count() > 0);
	}
	
	private void OnObjectDeselected(Class context, EditorObject target)
	{
		InfobarObjPosFrame.Show(GetEditor().GetObjectManager().GetSelectedObjects().Count() > 0);
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

