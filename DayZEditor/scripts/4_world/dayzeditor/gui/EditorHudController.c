
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
	ref ObservableCollection<Widget> LeftbarSpacer;
	ref ObservableCollection<Widget> RightbarSpacer;
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
		LeftbarSpacer 				= new ObservableCollection<Widget>("LeftbarSpacer", this);
		RightbarSpacer 				= new ObservableCollection<Widget>("RightbarSpacer", this);
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
		LeftbarSpacer.Insert(target.GetRoot());
	}	
	
	void InsertPlacedObject(EditorListItem target)
	{
		EditorLog.Trace("EditorHudController::InsertPlacedObject");
		RightbarSpacer.Insert(target.GetRoot());
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
					Widget list_item = LeftbarSpacer.Get(i);
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
	
	

	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::OnMouseEnter");
		
		
		switch (w.GetName()) {
			
			case "SnapButton":
			case "GroundButton":
			case "MagnetButton":
			case "UndoButton":
			case "RedoButton": {
				w.SetColor(COLOR_SALMON_A);
				return true;
			}			
		}
		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::OnMouseLeave");
		
		Widget icon = w.FindAnyWidget(string.Format("%1_Icon", w.GetName()));
		switch (w.GetName()) {
			
			case "UndoButton":
			case "RedoButton":
			case "MagnetButton":
			case "GroundButton":
			case "SnapButton": {
				w.SetColor(COLOR_EMPTY);
				return true;
			}
		}
			
		return false;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnMouseButtonDown");
		if (button != 0) return false;

#ifndef COMPONENT_SYSTEM
		if (GetEditor().IsPlacing()) {
			GetEditor().PlaceObject();
			return true;
		}
		
		GetEditor().ClearSelection();
#endif
		
		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnMouseButtonUp");
		if (button != 0) return false;
		
		Widget icon = w.FindAnyWidget(string.Format("%1_Icon", w.GetName()));
		switch (w.GetName()) {
			
			case "UndoButton": 
			case "RedoButton": {
				w.SetColor(COLOR_EMPTY);
				ButtonWidget.Cast(w).SetState(false);
				icon.SetPos(0, 0);
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
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnClick");
		if (button != 0) return false;
		
		Widget icon = w.FindAnyWidget(string.Format("%1_Icon", w.GetName()));
		
		switch (w.GetName()) {
			
			case "UndoButton": 
			case "RedoButton": {
				w.SetColor(COLOR_SALMON_A);
				int pos = ButtonWidget.Cast(w).GetState() * 1;
				icon.SetPos(pos, pos);
				break;
			}
			
			case "SnapButton":
			case "GroundButton":
			case "MagnetButton": {
				bool button_state = ButtonWidget.Cast(w).GetState();
				icon.SetColor((GetHighlightColor(w.GetName()) * button_state) - 1);
				icon.SetPos(button_state * 1, button_state * 1);
				break;
			}
		}
		
		return false;
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



