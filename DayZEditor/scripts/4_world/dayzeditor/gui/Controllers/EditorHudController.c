
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
	
	float BrushRadius = 50;
	float BrushDensity = 0.5;
	
	bool BrushToggleButtonState;
	int BrushTypeSelection;
	string BrushToggleButtonText;
	
	bool MagnetButton;
	bool GroundButton;
	bool SnapButton;
	
	float cam_x, cam_y, cam_z;	
	float obj_x, obj_y, obj_z;
	
	bool CinemaModeState;
	
	ref ObservableCollection<ref EditorWidget> LeftbarSpacerData;
	ref ObservableCollection<ref EditorWidget> RightbarSpacerData;
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData;
	ref ObservableCollection<string> DebugActionStackListbox;
	
	// View Properties
	protected Widget LeftbarFrame;
	protected ScrollWidget LeftbarScroll;
	
	protected Widget RightbarFrame;
	protected ButtonWidget BrushToggleButton;
	
	protected Widget NotificationFrame;
	protected Widget NotificationPanel;
	protected TextWidget NotificationText;
	
	CanvasWidget EditorCanvas;
	
	protected ButtonWidget MenuBarFile;
	protected ButtonWidget MenuBarEdit;
	protected ButtonWidget MenuBarView;
	
	protected WrapSpacerWidget LeftbarPanelSelectorWrapper;
	protected RadioButtonGroup m_RadioButtonGroup;
	
	void EditorHudController() {
		EditorLog.Trace("EditorHudController");
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
				
		LeftbarSpacerData 			= new ObservableCollection<ref EditorWidget>("LeftbarSpacerData", this);
		RightbarSpacerData 			= new ObservableCollection<ref EditorWidget>("RightbarSpacerData", this);
		BrushTypeBoxData 			= new ObservableCollection<ref EditorBrushData>("BrushTypeBoxData", this);
		DebugActionStackListbox 	= new ObservableCollection<string>("DebugActionStackListbox", this);
		
		LeftbarPanelSelectorWrapper.GetScript(m_RadioButtonGroup);
		m_RadioButtonGroup.OnRadioButtonActivate.Insert(OnRadioButtonActivate);
		m_RadioButtonGroup.OnRadioButtonDeactivate.Insert(OnRadioButtonDeactivate);
		
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
		LeftbarSpacerData.Insert(target);
	}	
	
	void InsertPlacedObject(EditorListItem target)
	{
		EditorLog.Trace("EditorHudController::InsertPlacedObject");
		RightbarSpacerData.Insert(target);
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
						
			case "BrushTypeSelection": {
				if (BrushToggleButtonState) {
					GetEditor().SetBrush(EditorBrush.Create(BrushTypeBoxData[BrushTypeSelection]));
				} else {
					GetEditor().SetBrush(null);
				}
				break;
			}
			
			case "BrushTypeBoxData": {
				BrushToggleButtonText = BrushTypeBoxData.Get(BrushTypeSelection).Name;
				NotifyPropertyChanged("BrushToggleButtonText");
				break;
			}
			
			case "BrushRadius":
			case "BrushDensity": {
				EditorBrush.SetRadius(BrushRadius);
				EditorBrush.SetDensity(BrushDensity);
				break;
			}
			
			case "SearchBarData": {
				
				for (int j = 0; j < LeftbarSpacerData.Count(); j++) {
					EditorPlaceableListItem placeable_item = LeftbarSpacerData.Get(j);
					placeable_item.GetLayoutRoot().Show(placeable_item.GetData().FilterType(SearchBarData));
				}
				
				LeftbarScroll.HScrollToPos(0);
				
				break;
			}			
		}
	}
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		EditorLog.Trace("EditorHudController::CollectionChanged: " + collection_name);
		switch (collection_name) {
			
			case "BrushTypeBoxData": {
				BrushToggleButtonText = BrushTypeBoxData.Get(BrushTypeSelection).Name;
				NotifyPropertyChanged("BrushToggleButtonText");
				break;
			}
		}
		
	}
	
	
	void LeftbarHideExecute(ButtonCommandArgs args) 
	{
		LeftbarFrame.Show(args.GetButtonState());
	}
	
	void RightbarHideExecute(ButtonCommandArgs args) 
	{
		RightbarFrame.Show(args.GetButtonState());
	}
	
	void BrushToggleExecute(ButtonCommandArgs args)
	{
		if (args.GetButtonState()) {
			GetEditor().SetBrush(EditorBrush.Create(BrushTypeBoxData[BrushTypeSelection]));
		} else {
			GetEditor().SetBrush(null);
		}
	}
	
	void ButtonCreateFolderExecute(ButtonCommandArgs args) {
		EditorLog.Trace("EditorHudController::ButtonCreateFolderExecute");
		EditorCollapsibleListItem category();
		InsertPlacedObject(category);
	}
	
	void WeatherExecute(ButtonCommandArgs args) {
		EditorWeatherDialog weather_dialog = new EditorWeatherDialog();
		weather_dialog.Show();
	}
	
	void CameraExecute(ButtonCommandArgs args) {
		EditorCameraDialog camera_dialog = new EditorCameraDialog(GetEditor().GetCamera());
		camera_dialog.Show();
	}
	
	void SettingsExecute(ButtonCommandArgs args) {
		EditorSettingsDialog settings_dialog = new EditorSettingsDialog(GetEditor().GetSettings());
		settings_dialog.Show();
	}

	void MenuBarFileExecute(ButtonCommandArgs args) 
	{
		EditorLog.Trace("EditorHudController::MenuBarFileExecute");
		CreateToolbarMenu(MenuBarFile);
	}
	
	void MenuBarEditExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorHudController::MenuBarEditExecute");
		CreateToolbarMenu(MenuBarEdit);
	}
	
	void MenuBarViewExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorHudController::MenuBarViewExecute");
		CreateToolbarMenu(MenuBarView);
	}
	
	private EditorMenu CreateToolbarMenu(Widget toolbar_button)
	{
		EditorLog.Trace("EditorHudController::CreateToolbarMenu");
		float x, y, w, h;
		toolbar_button.GetScreenPos(x, y);
		toolbar_button.GetScreenSize(w, h);
		y += h;
		
		/*
		if (GetEditor().GetEditorHud().IsMenuActive()) {
			GetEditor().GetEditorHud().GetMenu().Close();
			return;
		}*/
		
		EditorMenu toolbar_menu = GetBoundMenu(toolbar_button).Spawn();
		toolbar_menu.SetPosition(x, y);
		toolbar_menu.Show();
		return toolbar_menu;
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

	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		EditorLog.Trace("EditorHudController::OnMouseWheel");
		
		string w_name = w.GetName();
		float direction = wheel;
		switch (w_name) {
			
			case "pos":
			case "rot": {
			
				StringEvaluater w_eval;
				EnScript.GetClassVar(this, w_name, 0, w_eval);
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					direction *= 10;
				} else if (KeyState(KeyCode.KC_LMENU)) {
					direction /= 10;
				}
				
				EnScript.SetClassVar(this, w_name, 0, (w_eval.Parse() + direction).ToString());
				NotifyPropertyChanged(w_name);
				break;
			}
			
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
		
		return false;
	}
	
	
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::OnMouseEnter %1", w.GetName());		
		
		if (GetEditor() && !GetEditor().GetEditorHud().ShouldProcessInput(w)) {
			return false;
		}
		
		// Specific style handlers
		switch (w) {
			case MenuBarFile:
			case MenuBarEdit:
			case MenuBarView: {
				w.SetColor(COLOR_SALMON);
				EditorHud editor_hud = GetEditor().GetEditorHud();
				if (editor_hud.IsMenuActive() && !editor_hud.GetMenu().IsInherited(EditorContextMenu)) {
					editor_hud.CloseMenu();
					CreateToolbarMenu(w);
				}
				
				return false;
			}
		}
		
		switch (w.GetTypeName()) {
			
			case "ButtonWidget": {
				w.SetColor(COLOR_SALMON);				
				break;
			}
			
			case "SliderWidget": {
				w.SetColor(COLOR_SALMON);
				break;
			}
		}
		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::OnMouseLeave %1", w.GetName());
		
		switch (w.GetTypeName()) {
			
			case "ButtonWidget": {
				if (!ButtonWidget.Cast(w).GetState())
					w.SetColor(COLOR_EMPTY);
				
				break;
			}

			case "SliderWidget": {
				w.SetColor(COLOR_WHITE_A);
				break;
			}
		}
		
		return false;
			
	}
	
	

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnMouseButtonUp");
		
		if (button != 0) return false;

		switch (w.GetTypeName()) {

			case "ButtonWidget": {
				w.SetColor(COLOR_EMPTY);
				ButtonWidget.Cast(w).SetState(false);
				SetWidgetIconPosition(w, 0, 0);
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
				
					
					case "EditorListItemButton": {
						
						
						break;
					}
				}
							
				break;
			}
			
			case 1: {
				
				switch (w.GetName()) {
								
					case "BrushToggleButton": {
						EditorBrushDialog brush_dialog(BrushTypeBoxData[BrushTypeSelection]);
						brush_dialog.Show();
						break;
					}
				}
				
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
				
		root.SetPos(Math.Lerp(start_x, final_x, normalized_time), Math.Lerp(start_y, final_y, normalized_time));
		
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
				list_item.Show(item.GetData().GetCategory() == radio_button.GetID());
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
	
		
	typename GetBoundMenu(Widget target)
	{
		switch (target) {
			
			case MenuBarFile: {
				return EditorFileMenu;
			}
			
			case MenuBarEdit: {
				return EditorEditMenu;
			}
			
			case MenuBarView: {
				return EditorViewMenu;
			}
		}
		
		return typename;
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



