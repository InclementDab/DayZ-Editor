

static ref EditorUIViewModel m_EditorUIViewModel;
EditorUIViewModel GetEditorUIViewModel() { return m_EditorUIViewModel; }

// 	  vvvvvvvvvvvvvvvvv  put THIS into ScriptClass
class EditorUIViewModel: ViewModelBase
{
	
	string DebugText1;
	string DebugText2;
	string DebugText3;
	string DebugText4;
	string DebugText5;
	
	bool BuildingSelect;
	bool VehicleSelect;
	bool EntitySelect;
	bool HumanSelect;
	
	
	void EditorUIViewModel()
	{
		EditorLog.Trace("EditorUIViewModel");
		m_EditorUIViewModel = this;
	}
	
	override void OnPropertyChanged(string property_name)
	{
		if (BuildingSelect) {
			VehicleSelect = false; EntitySelect = false; HumanSelect = false;
			GetEditor().UpdatePlaceableItems(PlaceableObjectCategory.BUILDING);
		}
		else if (VehicleSelect) {
			BuildingSelect = false; EntitySelect = false; HumanSelect = false;
			GetEditor().UpdatePlaceableItems(PlaceableObjectCategory.VEHICLE);
		}
		else if (EntitySelect) {
			BuildingSelect = false; VehicleSelect = false; HumanSelect = false;
			GetEditor().UpdatePlaceableItems(PlaceableObjectCategory.ENTITY);
		}
		else if (HumanSelect) {
			BuildingSelect = false; VehicleSelect = false; EntitySelect = false;
			GetEditor().UpdatePlaceableItems(PlaceableObjectCategory.HUMAN);
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
	
	void OnPropertyChanged(string property_name) {}
}



