
class EditorHudController: Controller
{

	// Data Binding
	string DebugText1;
	string DebugText2;
	string DebugText3;
	string DebugText4;
	string DebugText5;
	
	int PlaceableCategorySelection = 0;
	
	bool LeftbarHide;
	bool RightbarHide;
	
	float BrushRadius = 50;
	float BrushDensity = 0.5;
	
	bool BrushToggleButton;
	
	bool MagnetButton;
	bool GroundButton;
	bool SnapButton;
	
	//ref TextListboxWidgetData DebugActionStackListbox;
	ref ObservableCollection<Widget> LeftbarSpacer;
	ref ObservableCollection<Widget> RightbarSpacer;
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData;
	ref ObservableCollection<string> DebugActionStackListbox;
	
	int BrushTypeSelection;
	
	// View Properties
	protected Widget LeftbarFrame;
	protected Widget RightbarFrame;
	protected XComboBoxWidget BrushTypeBox;
	
	
	void EditorHudController()
	{
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
		}
	}
	
	void SelectBrush(bool state)
	{
#ifndef COMPONENT_SYSTEM
		if (state) {
			GetEditor().SetBrush(EditorBrush.Create(BrushTypeBoxData[BrushTypeBox.GetCurrentItem()]));
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
	
	bool CanBrushToggleExecute() 
	{
		return true;
		return (BrushTypeBoxData.Count() > 0);
	}
	
	void BrushTypeBoxExecute(XComboBoxCommandArgs args)
	{
		EditorLog.Trace("EditorHudController::BrushTypeBoxExecute");
		SelectBrush(BrushToggleButton);
	}
	
	void ToggleLeftBar(ButtonCommandArgs args) {
		float x, y;
		LeftbarFrame.GetPos(x, y);
		LeftbarFrame.SetPos(-300 * args.param3, y);
	}
	
	void ToggleRightBar(ButtonCommandArgs args) {
		float x, y;
		RightbarFrame.GetPos(x, y);
		RightbarFrame.SetPos(-300 * args.param3, y);
	}
	
	void LeftbarSpacerExecute(WrapSpacerCommandArgs args) {
		EditorLog.Trace("EditorHudController::LeftbarSpacerExecute");
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnMouseButtonDown");
		if (button == 0) {

#ifndef COMPONENT_SYSTEM
			
			if (GetEditor().IsPlacing()) {
				GetEditor().PlaceObject();
				return true;
			}
			
			GetEditor().ClearSelection();
#endif
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
}




