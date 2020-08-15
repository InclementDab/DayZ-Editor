



static ref EditorUIViewModel m_EditorUIViewModel;
EditorUIViewModel GetEditorUIViewModel() { return m_EditorUIViewModel; }

// 	  vvvvvvvvvvvvvvvvv  put THIS into ScriptClass
class EditorUIViewModel: ViewModelBase
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
	
	ref TextListboxWidgetData WrapSpacerTest;
	
	void EditorUIViewModel()
	{
		EditorLog.Trace("EditorUIViewModel");
		m_EditorUIViewModel = this;
		WrapSpacerTest = new TextListboxWidgetData();
		// Load PlaceableObjects
		/*
		m_PlaceableObjects = new EditorPlaceableListItemSet();
		EditorLog.Info(string.Format("Loaded %1 Placeable Objects", GetPlaceableObjects(m_PlaceableObjects)));
		foreach (ref EditorPlaceableListItem placeable_object: m_PlaceableObjects) {
			//m_EditorUI.InsertPlaceableObject(placeable_object);
		}*/
				
		UpdateViews();
		
	}
	
	static int GetPlaceableObjects(out EditorPlaceableListItemSet placeable_objects) 
	{ 
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);

		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < GetGame().ConfigGetChildrenCount(Config_Path); j++) {
				string Config_Name, Base_Name;
		        GetGame().ConfigGetChildName(Config_Path, j, Config_Name); 
				
				EditorPlaceableObjectData placeable_object_data = new EditorPlaceableObjectData(Config_Name, Config_Path);
				placeable_objects.Insert(new EditorPlaceableListItem(placeable_object_data));
		    }
		}
		
		return placeable_objects.Count();
	}
	
	void UpdatePlaceableItems(PlaceableObjectCategory category)
	{
		EditorLog.Trace("EditorUIViewModel::UpdatePlaceableItems");
		foreach (EditorPlaceableListItem placeable_object: m_PlaceableObjects) {
			Widget root = placeable_object.GetRoot();
			root.Show(placeable_object.GetData().GetCategory() == category);
		}
	}
	
	override void OnPropertyChanged(Widget target)
	{
		Print("OnPropertyChanged: " + target.GetName());
		WrapSpacerTest.Insert(target.GetName(), target);
		switch (target.GetName()) {
			
			case "BuildingSelect": {
				VehicleSelect = false; EntitySelect = false; HumanSelect = false;
				//UpdatePlaceableItems(PlaceableObjectCategory.HUMAN);
				break;
			}
			
			case "VehicleSelect": {
				BuildingSelect = false; EntitySelect = false; HumanSelect = false;
				//UpdatePlaceableItems(PlaceableObjectCategory.HUMAN);
				break;
			}
			
			case "EntitySelect": {
				BuildingSelect = false; VehicleSelect = false; HumanSelect = false;
				//UpdatePlaceableItems(PlaceableObjectCategory.HUMAN);
				break;
			}
			
			case "HumanSelect": {
				BuildingSelect = false; VehicleSelect = false; EntitySelect = false;
				//UpdatePlaceableItems(PlaceableObjectCategory.HUMAN);
				break;
			}
			
			
		}
		
	
		
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
}



