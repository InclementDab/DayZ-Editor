
class EditorHudController: Controller
{

	// Data Binding
	string DebugText1;
	string DebugText2;
	string DebugText3;
	string DebugText4;
	string DebugText5;
	string DebugText6;
	
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
	
	//ref TextListboxWidgetData DebugActionStackListbox;
	ref ObservableCollection<ref EditorWidget> LeftbarSpacer;
	ref ObservableCollection<ref EditorWidget> RightbarSpacer;
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData;
	ref ObservableCollection<string> DebugActionStackListbox;
	
	
	
	// View Properties
	protected Widget LeftbarFrame;
	protected Widget RightbarFrame;
	protected ButtonWidget BrushToggleButton;

	
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
				SelectBrush(BrushToggleButtonState);
				break;
			}
			
			case "BrushTypeBoxData": {
				BrushToggleButton.SetText(BrushTypeBoxData.Get(BrushTypeSelection).Name);
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
	
	void SelectBrush(bool state)
	{
#ifndef COMPONENT_SYSTEM
		if (state) {
			GetEditor().SetBrush(EditorBrush.Create(BrushTypeBoxData[BrushTypeSelection]));
		} else {
			GetEditor().SetBrush(null);
		}	
#endif
	}
	
	
	void BrushToggleExecute(ButtonCommandArgs args) 
	{
		EditorLog.Trace("EditorHudController::BrushToggleExecute");
		SelectBrush(args.param3);
	}
	
	
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

	
	override void MVCOnMouseEnter(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorHudController::MVCOnMouseEnter %1", target.GetName());		
		switch (target.GetName()) {
			
			case "SnapButton":
			case "GroundButton":
			case "MagnetButton":
			case "UndoButton":
			case "RedoButton": {
				target.SetColor(COLOR_SALMON_A);
				break;
			}
			
			case "EditorListItemHighlight": {
				target.Show(true);
				break;
			}
		}
	}
	
	override void MVCOnMouseLeave(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorHudController::MVCOnMouseLeave %1", target.GetName());
		
		Widget icon = target.FindAnyWidget(string.Format("%1_Icon", target.GetName()));
		switch (target.GetName()) {
			
			case "UndoButton":
			case "RedoButton":
			case "MagnetButton":
			case "GroundButton":
			case "SnapButton": {
				target.SetColor(COLOR_EMPTY);
				break;
			}
			
			case "EditorListItemHighlight": {
				target.Show(false);
				break;
			}
		}
			
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
		if (button != 0) return;
		
		Widget icon = target.FindAnyWidget(string.Format("%1_Icon", target.GetName()));
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



