
static ref EditorHudController m_EditorHudController;
EditorHudController GetEditorHudController() { return m_EditorHudController; }



class EditorHudController: Controller
{
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
	
	//ref TextListboxWidgetData DebugActionStackListbox;
	ref ObservableCollection<Widget> LeftbarSpacer;
	ref ObservableCollection<Widget> RightbarSpacer;
	ref ObservableCollection<string> BrushTypeBox;
	ref ObservableCollection<string> DebugActionStackListbox;
	
	//ref XComboBoxWidgetData BrushTypeBox;
	
	void EditorHudController()
	{
		EditorLog.Trace("EditorHudController");
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		DebugActionStackListbox 	= new ObservableCollection<string>("DebugActionStackListbox", this);
		BrushTypeBox				= new ObservableCollection<string>("BrushTypeBox", this);
		LeftbarSpacer 				= new ObservableCollection<Widget>("LeftbarSpacer", this);
		RightbarSpacer 				= new ObservableCollection<Widget>("RightbarSpacer", this);
		
		// Reload Placeables
		EditorLog.Info(string.Format("Loaded %1 Placeable Objects", ReloadPlaceableObjects()));
		
		
		// Load Brushes
		m_EditorBrushTypes = new EditorBrushDataSet();
		m_CustomBrushList = new map<string, typename>();
		ReloadBrushes("$profile:Editor/EditorBrushes.xml");
		RegisterCustomBrush("Delete", DeleteBrush);
	}
	
		
	// Brush Management
	private ref EditorBrushDataSet 		m_EditorBrushTypes;
	private ref map<string, typename> 	m_CustomBrushList;
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
	
	EditorBrushData GetLoadedBrushData(string name) { return m_EditorBrushTypes.Get(name); }
	void RegisterCustomBrush(string name, typename type) { m_CustomBrushList.Insert(name, type); }
	
	void SetBrushTypes(EditorBrushDataSet brush_types)
	{
		EditorLog.Trace("EditorHudController::SetBrushTypes");
		m_EditorBrushTypes = brush_types;

		foreach (EditorBrushData brush: m_EditorBrushTypes)
			BrushTypeBox.Insert(brush.Name);	
	}
		
	EditorBrush GetBrush(string brush_name)
	{
		EditorLog.Trace("EditorSettings::GetBrush " + brush_name);
		foreach (EditorBrushData settings: m_EditorBrushTypes) {
			if (settings.Name == brush_name) {
				foreach (string name, typename type: m_CustomBrushList) 
					if (name == brush_name)
						return type.Spawn();
					
				return new EditorBrush(settings);
			}
		}
		
		EditorLog.Trace("EditorSettings::GetBrush Brush not found!");
		return null;
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
		if (property_name == "PlaceableCategorySelection") {
			
			for (int i = 0; i < LeftbarSpacer.Count(); i++) {
				Widget list_item = LeftbarSpacer.Get(i);
				EditorPlaceableListItem item;
				list_item.GetUserData(item);
				list_item.Show(item.GetData().GetCategory() == PlaceableCategorySelection);
			}
		}
		
	}
}




