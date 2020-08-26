
static ref EditorHudController m_EditorHudController;
EditorHudController GetEditorHudController() { return m_EditorHudController; }



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
	
	bool BrushToggleButton;
	
	float BrushRadius = 50;
	float BrushDensity = 0.5;
	
	//ref TextListboxWidgetData DebugActionStackListbox;
	ref ObservableCollection<Widget> LeftbarSpacer;
	ref ObservableCollection<Widget> RightbarSpacer;
	ref ObservableCollection<ref EditorBrushData> BrushTypeBox;
	ref ObservableCollection<string> DebugActionStackListbox;
	
	
	
	void EditorHudController()
	{
		EditorLog.Trace("EditorHudController");
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		DebugActionStackListbox 	= new ObservableCollection<string>("DebugActionStackListbox", this);
		BrushTypeBox				= new ObservableCollection<ref EditorBrushData>("BrushTypeBox", this);
		LeftbarSpacer 				= new ObservableCollection<Widget>("LeftbarSpacer", this);
		RightbarSpacer 				= new ObservableCollection<Widget>("RightbarSpacer", this);
		
		// Reload Placeables
		EditorLog.Info(string.Format("Loaded %1 Placeable Objects", ReloadPlaceableObjects()));
		
		
		// Load Brushes
		ReloadBrushes("$profile:Editor/EditorBrushes.xml");
	}
	
		
	// Brush Management
	void ReloadBrushes(string filename)
	{
		EditorLog.Trace("EditorHudController::ReloadBrushes");
		XMLEditorBrushes xml_brushes(BrushTypeBox);
		
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

#ifndef COMPONENT_SYSTEM
			case "BrushToggleButton": {
				if (BrushToggleButton) {
					int index = XComboBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("BrushTypeBox")).GetCurrentItem();
					GetEditor().SetBrush(EditorBrush.Create(BrushTypeBox[index]));
				} else {
					GetEditor().SetBrush(null);
				}
				break;
			}
#endif
		}
	}
}




