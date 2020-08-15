



static ref EditorHudViewModel m_EditorHudViewModel;
EditorHudViewModel GetEditorHudViewModel() { return m_EditorHudViewModel; }

// 	  vvvvvvvvvvvvvvvvv  put THIS into ScriptClass
class EditorHudViewModel: ViewModelBase
{
	private ref EditorPlaceableListItemSet m_PlaceableObjects;
	
	
	string DebugText1;
	string DebugText2;
	string DebugText3;
	string DebugText4;
	string DebugText5;
	
	bool BuildingSelect;
	bool VehicleSelect;
	bool EntitySelect;
	bool HumanSelect;
	
	ref TextListboxWidgetData DebugActionStackListbox;
	ref WrapSpacerWidgetData LeftbarSpacer;
	ref WrapSpacerWidgetData RightbarSpacer;
	ref XComboBoxWidgetData BrushTypeBox;
	
	void EditorHudViewModel()
	{
		EditorLog.Trace("EditorHudViewModel");
		m_EditorHudViewModel = this;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		DebugActionStackListbox 	= new TextListboxWidgetData();
		LeftbarSpacer 				= new WrapSpacerWidgetData();
		RightbarSpacer 				= new WrapSpacerWidgetData();
		BrushTypeBox				= new XComboBoxWidgetData();
		
		// Load PlaceableObjects
		EditorLog.Info(string.Format("Loaded %1 Placeable Objects", ReloadPlaceableObjects()));
		
		BrushTypeBox.Insert("Brush1");
		BrushTypeBox.Insert("Brush2");
		BrushTypeBox.Insert("Brush3");
		
		UpdateViews();		
	}
	
	
	void InsertPlaceableObject(EditorListItem target)
	{
		m_PlaceableObjects.Insert(target);
		LeftbarSpacer.InsertWidget(target.GetRoot());
	}	
	
	void InsertPlacedObject(EditorListItem target)
	{
		RightbarSpacer.Insert(target.GetRoot());
	}
	
	void InsertMapObject(Widget map_marker)
	{
		Print("EditorHudViewModel::InsertMapObject " + map_marker.GetName());
		//m_EditorMapWidget.AddChild(map_marker);
	}
	
	int ReloadPlaceableObjects() 
	{ 
		m_PlaceableObjects = new EditorPlaceableListItemSet();
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);
		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < GetGame().ConfigGetChildrenCount(Config_Path); j++) {
				string Config_Name, Base_Name;
		        GetGame().ConfigGetChildName(Config_Path, j, Config_Name);
				EditorPlaceableObjectData placeable_object_data = new EditorPlaceableObjectData(Config_Name, Config_Path);
				InsertPlaceableObject(new EditorPlaceableListItem(placeable_object_data));
		    }
		}
		
		return j;
	}
	
	void UpdatePlaceableItems(PlaceableObjectCategory category)
	{
		EditorLog.Trace("EditorUIViewModel::UpdatePlaceableItems");
		foreach (EditorPlaceableListItem placeable_object: m_PlaceableObjects) {
			Widget root = placeable_object.GetRoot();
			root.Show(placeable_object.GetData().GetCategory() == category);
		}
		
	}
	
	void ClearBrushBox()
	{
		BrushTypeBox.Clear();
	}
	
	void InsertBrush(string name)
	{
		BrushTypeBox.Insert(name);
	}
	
	override bool OnClick(Widget w, int x, int y, bool button) 
	{
		switch (w.GetName()) {
			
			case "BuildingSelect": {
				VehicleSelect = false; EntitySelect = false; HumanSelect = false;
				UpdatePlaceableItems(PlaceableObjectCategory.BUILDING);
				break;
			}
			
			case "VehicleSelect": {
				BuildingSelect = false; EntitySelect = false; HumanSelect = false;
				UpdatePlaceableItems(PlaceableObjectCategory.VEHICLE);
				break;
			}
			
			case "EntitySelect": {
				BuildingSelect = false; VehicleSelect = false; HumanSelect = false;
				UpdatePlaceableItems(PlaceableObjectCategory.ENTITY);
				break;
			}
			
			case "HumanSelect": {
				BuildingSelect = false; VehicleSelect = false; EntitySelect = false;
				UpdatePlaceableItems(PlaceableObjectCategory.HUMAN);
				break;
			}
		}
		
		UpdateViews();
		
		return super.OnClick(w, x, y, button);
	}
	
	
	override void OnPropertyChanged(Widget target)
	{
		EditorLog.Trace("OnPropertyChanged: " + target.GetName());

	
		UpdateViews();
	}
}
	


typedef ref map<string, typename> VariableHashMap;


// abstract
class ViewModelBase: Managed
{
	protected Widget m_LayoutRoot;
	
	// Hashed Variable Data
	protected ref VariableHashMap m_ModelHashMap;
	VariableHashMap GetVariableHashMap() { return m_ModelHashMap; }
	typename GetVariableType(string var_name) { return m_ModelHashMap.Get(var_name); }
	
	// View List
	protected ref array<ref EditorView> m_ViewList;
	
	void ViewModelBase()
	{
		EditorLog.Trace("ViewModelBase");
		m_ModelHashMap = new VariableHashMap();
		m_ViewList = new array<ref EditorView>();

		typename vtype = Type();
		int vcnt = vtype.GetVariableCount();
		for (int i = 0; i < vcnt; i++) {
			m_ModelHashMap.Insert(vtype.GetVariableName(i), vtype.GetVariableType(i));
		}
	}
	
	void ~ViewModelBase() { EditorLog.Trace("~ViewModelBase"); }
	
		
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("ViewModelBase::OnWidgetScriptInit");	
		m_LayoutRoot = w;
	}
	

	void UpdateViews()
	{
		foreach (ref EditorView view: m_ViewList)
			view.UpdateView();
	}
	
	void InsertView(ref EditorView view)
	{
		EditorLog.Trace("ViewModelBase::InsertView: " + view);
		m_ViewList.Insert(view);
	}
	
	void DebugPrint()
	{
		EditorLog.Debug("ViewModelBase::DebugPrint: " + m_LayoutRoot.GetName());
		foreach (ref EditorView view: m_ViewList)
			view.DebugPrint();
	}
	
	void OnPropertyChanged(Widget target) {}
	
	bool OnClick(Widget w, int x, int y, bool button) { return true; }
}



