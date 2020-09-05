
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
	
	bool LeftbarHide;
	bool RightbarHide;
	
	float BrushRadius = 50;
	float BrushDensity = 0.5;
	
	bool BrushToggleButtonState;
	int BrushTypeSelection;
	
	bool MagnetButton;
	bool GroundButton;
	bool SnapButton;
	
	float cam_x, cam_y, cam_z;	
	float obj_x, obj_y, obj_z;
	
	//ref TextListboxWidgetData DebugActionStackListbox;
	ref ObservableCollection<ref EditorWidget> LeftbarSpacer;
	ref ObservableCollection<ref EditorWidget> RightbarSpacer;
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData;
	ref ObservableCollection<string> DebugActionStackListbox;
	
	// View Properties
	protected Widget LeftbarFrame;
	protected Widget RightbarFrame;
	protected ButtonWidget BrushToggleButton;
	
	protected Widget NotificationFrame;
	protected Widget NotificationPanel;
	protected TextWidget NotificationText;
	
	CanvasWidget EditorCanvas;

	
	void EditorHudController() {
		EditorLog.Trace("EditorHudController");
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
				
		DebugActionStackListbox 	= new ObservableCollection<string>("DebugActionStackListbox", this);
		LeftbarSpacer 				= new ObservableCollection<ref EditorWidget>("LeftbarSpacer", this);
		RightbarSpacer 				= new ObservableCollection<ref EditorWidget>("RightbarSpacer", this);
		BrushTypeBoxData 			= new ObservableCollection<ref EditorBrushData>("BrushTypeBoxData", this);
		
		// Reload Placeables
		EditorLog.Info(string.Format("Loaded %1 Placeable Objects", ReloadPlaceableObjects()));
		
		// Load Brushes
		ReloadBrushes("$profile:Editor/EditorBrushes.xml");
	}
	
		
	// Brush Management
	void ReloadBrushes(string filename)
	{
		EditorLog.Trace("EditorHudController::ReloadBrushes");
		XMLEditorBrushes xml_brushes(BrushTypeBoxData);
		
		if (!FileExist(filename)) {
			EditorLog.Error("File not found: " + filename);
			return;
		}
	
		GetXMLApi().Read(filename, xml_brushes);
	}

	
		

	
	void InsertPlaceableObject(EditorListItem target)
	{
		EditorLog.Trace("EditorHudController::InsertPlaceableObject");
		LeftbarSpacer.Insert(target);
	}	
	
	void InsertPlacedObject(EditorListItem target)
	{
		EditorLog.Trace("EditorHudController::InsertPlacedObject");
		RightbarSpacer.Insert(target);
	}
	
	void InsertMapMarker(EditorMarker map_marker)
	{
		Print("EditorHudController::InsertMapObject " + map_marker.GetLayoutRoot().GetName());
		GetEditor().GetEditorHud().GetMapWidget().AddChild(map_marker.GetLayoutRoot());
	}
	
	int ReloadPlaceableObjects() 
	{ 
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);
		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < GetGame().ConfigGetChildrenCount(Config_Path); j++) {
				string Config_Name, Base_Name;
		        GetGame().ConfigGetChildName(Config_Path, j, Config_Name);
				EditorPlaceableObjectData placeable_object_data = new EditorPlaceableObjectData(Config_Name, Config_Path);
#ifndef COMPONENT_SYSTEM
				InsertPlaceableObject(new EditorPlaceableListItem(placeable_object_data));
#endif
		    }
		}
		
		return j;
	}
	
	

	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorHudController::PropertyChanged: " + property_name);
		
		switch (property_name) {
			
			case "PlaceableCategorySelection": {
			
				for (int i = 0; i < LeftbarSpacer.Count(); i++) {
					Widget list_item = LeftbarSpacer[i].GetLayoutRoot();
					EditorPlaceableListItem item;
					list_item.GetUserData(item);
					list_item.Show(item.GetData().GetCategory() == PlaceableCategorySelection);
				}
				break;
			}
			
			case "BrushTypeSelection": {
				if (BrushToggleButtonState) {
					GetEditor().SetBrush(EditorBrush.Create(BrushTypeBoxData[BrushTypeSelection]));
				} else {
					GetEditor().SetBrush(null);
				}
				break;
			}
			
			case "BrushTypeBoxData": {
				BrushToggleButton.SetText(BrushTypeBoxData.Get(BrushTypeSelection).Name);
				break;
			}
			
			case "BrushRadius":
			case "BrushDensity": {
				EditorBrush.SetRadius(BrushRadius);
				EditorBrush.SetDensity(BrushDensity);
				break;
			}
			
			case "SearchBarData": {
				
				for (int j = 0; j < LeftbarSpacer.Count(); j++) {
					EditorPlaceableListItem placeable_item = LeftbarSpacer.Get(j);
					
					placeable_item.GetLayoutRoot().Show(placeable_item.GetData().FilterType(SearchBarData));
				}
				
				break;
			}
			
			

		}
	}
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		EditorLog.Trace("EditorHudController::CollectionChanged: " + collection_name);
		switch (collection_name) {
			
			case "BrushTypeBoxData": {
				BrushToggleButton.SetText(BrushTypeBoxData.Get(BrushTypeSelection).Name);
				break;
			}
		}
		
	}
	
	
	/*
	void BrushToggleExecute(ButtonCommandArgs args) 
	{
		EditorLog.Trace("EditorHudController::BrushToggleExecute");
		SelectBrush(args.param3);
	}*/
	
	
	void ToggleLeftBar(ButtonCommandArgs args) 
	{
		float x, y;
		LeftbarFrame.GetPos(x, y);
		LeftbarFrame.SetPos(-300 * args.param3, y);
	}
	
	void ToggleRightBar(ButtonCommandArgs args) 
	{
		float x, y;
		RightbarFrame.GetPos(x, y);
		RightbarFrame.SetPos(-300 * args.param3, y);
	}
	
	void LeftbarSpacerExecute(WrapSpacerCommandArgs args) {
		EditorLog.Trace("EditorHudController::LeftbarSpacerExecute");
	}
	
	
	void ButtonCreateFolderExecute(ButtonCommandArgs args) {
		EditorLog.Trace("EditorHudController::ButtonCreateFolderExecute");
		EditorCollapsibleListItem category();
		InsertPlacedObject(category);
	}
	
	void WeatherExecute(ButtonCommandArgs args) {
		EditorWeatherDialog weather_dialog = new EditorWeatherDialog();
		weather_dialog.ShowDialog();
	}
	
	void CameraExecute(ButtonCommandArgs args) {
		EditorCameraDialog camera_dialog = new EditorCameraDialog(GetEditor().GetCamera());
		camera_dialog.ShowDialog();
	}
	
	
	
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
			*/
			
			
		}
		
		return false;
	}

	override void MVCOnMouseWheel(Widget target, int direction, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::MVCOnMouseWheel");
		
		switch (target.GetName()) {
			
			case "BrushRadiusText":
			case "BrushRadiusSlider": {
				BrushRadius += direction * 2;
				BrushRadius = Math.Clamp(BrushRadius, 0, 100);
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
		}
	}

	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorHudController::OnMouseEnter %1", w.GetName());		
		switch (w.GetName()) {
			
			case "SnapButton":
			case "GroundButton":
			case "MagnetButton":
			case "UndoButton":
			case "RedoButton": {
				w.SetColor(COLOR_SALMON_A);
				break;
			}
			
		
		}
		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		EditorLog.Trace("EditorHudController::OnMouseLeave %1", w.GetName());
		
		Widget icon = w.FindAnyWidget(string.Format("%1_Icon", w.GetName()));
		switch (w.GetName()) {
			
			case "UndoButton":
			case "RedoButton":
			case "MagnetButton":
			case "GroundButton":
			case "SnapButton": {
				w.SetColor(COLOR_EMPTY);
				break;
			}
		
		}
		
		return false;
			
	}
	

	
	override void MVCOnMouseUp(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorHudController::MVCOnMouseButtonUp");
		if (button != 0) return;
		
		Widget icon = target.FindAnyWidget(string.Format("%1_Icon", target.GetName()));
		switch (target.GetName()) {
			
			case "UndoButton": 
			case "RedoButton": {
				target.SetColor(COLOR_EMPTY);
				ButtonWidget.Cast(target).SetState(false);
				icon.SetPos(0, 0);
				break;
			}
		}
	}
		
	override void MVCOnClick(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorHudController::MVCOnClick");
		
		
		Widget icon = target.FindAnyWidget(string.Format("%1_Icon", target.GetName()));
		
		switch (button) {
			
			case 0: {
		
				switch (target.GetName()) {
					
					case "UndoButton": 
					case "RedoButton": {
						target.SetColor(COLOR_SALMON_A);
						int pos = ButtonWidget.Cast(target).GetState() * 1;
						icon.SetPos(pos, pos);
						break;
					}
					
					case "SnapButton":
					case "GroundButton":
					case "MagnetButton": {
						bool button_state = ButtonWidget.Cast(target).GetState();
						icon.SetColor((GetHighlightColor(target.GetName()) * button_state) - 1);
						icon.SetPos(button_state * 1, button_state * 1);
						break;
					}
					
					case "EditorListItemButton": {
						
						
						break;
					}
				}
							
				break;
			}
			
			case 1: {
				
				switch (target.GetName()) {
								
					case "BrushToggleButton": {
						EditorBrushDialog brush_dialog(BrushTypeBoxData[BrushTypeSelection]);
						brush_dialog.ShowDialog();
						break;
					}
				}
				
				break;
			}
		}
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
		}
		
		return 0;
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
			if (GetEditor().GetEditorHud().IsMapOpen()) {
				editor_object.GetMapMarkerPosition(marker_x, marker_y);
			} else {
				editor_object.GetObjectMarkerPosition(marker_x, marker_y);
			}
			
			
			if ((marker_x < x_high && marker_x > x_low) && (marker_y < y_high && marker_y > y_low)) {		
				GetEditor().SelectObject(editor_object);
			}
		}		
	}
	
	private float notification_start_x, notification_start_y;
	void NotificationCreate(string text, int color = -4301218, float duration = 4)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationAnimateFrame);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationDestroy);
		
				
		NotificationPanel.SetColor(color);
		NotificationText.SetText(text);
		NotificationFrame.Show(true);
			
		float width, height;
		NotificationFrame.GetSize(width, height);
		
		EffectSound notif_sound = SEffectManager.PlaySound("Notification_SoundSet", GetEditor().GetCamera().GetPosition());
		notif_sound.SetSoundAutodestroy(true);
		
		// Animate pulldown
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationAnimateFrame, 0, true, NotificationFrame, GetGame().GetTime(), 0.25, notification_start_x, notification_start_x, notification_start_y, notification_start_y + height);
		
		// Call destroy after duration done
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationDestroy, duration * 1000, false, NotificationFrame, notification_start_x, notification_start_y);
	}
	
	
	
	
	private void NotificationAnimateFrame(Widget root, float anim_starttime, float duration, float start_x, float final_x, float start_y, float final_y)
	{
				
		float anim_frametime = GetGame().GetTime() - anim_starttime;
		anim_frametime /= 1000;
		
		float normalized_time = (1 / duration) * anim_frametime;
		normalized_time = Math.Clamp(normalized_time, 0, 1);
		
		float pos_x = Math.Lerp(start_x, final_x, normalized_time);
		float pos_y = Math.Lerp(start_y, final_y, normalized_time);
		
		root.SetPos(pos_x, pos_y);
		
		if (normalized_time >= 1)
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationAnimateFrame);
		
		
	}
	
	private void NotificationDestroy(Widget root, float start_x, float start_y)
	{
		float current_x, current_y;
		root.GetPos(current_x, current_y);
		
		float duration = 0.25;
		// Animate in reverse
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationAnimateFrame, 0, true, root, GetGame().GetTime(), duration, current_x, start_x, current_y, start_y);
		
		// Hide Object
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(root.Show, duration * 1000, false, false);
	}
	
	
}


//static const int COLOR_BLUE 	= -13330213;
static const int COLOR_SALMON	= -4235425;
static const int COLOR_SALMON_A	= 2143248223;
static const int COLOR_CANDY 	= -1618884;
static const int COLOR_APPLE 	= -9785268;
static const int COLOR_JELLY 	= -1010901;
static const int COLOR_BLACK	= -16777216;
static const int COLOR_WHITE 	= -1;
static const int COLOR_WHITE_A	= 855638015;
static const int COLOR_EMPTY 	= 0;



