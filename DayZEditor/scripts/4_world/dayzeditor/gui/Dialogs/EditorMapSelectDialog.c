

static const ref array<string> ExcludedMapItems = {
	"access",
	"DefaultLighting",
	"DefaultWorld",
	"initWorld",
	"CAWorld"
};

class MapSelectDialogController: EditorDialogController
{
	protected TextListboxWidget MapHostListbox;
	
	ref ObservableCollection<string> MapHostListboxData = new ObservableCollection<string>("MapHostListboxData", this);
	string MapHostListboxSelection;
		
	void SetMapSelectDialog(MapSelectDialog select_dialog)
	{
		EditorLog.Trace("MapSelectDialogController::SetMapSelectDialog");
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (ExcludedMapItems.Find(name) == -1) {
				MapHostListboxData.Insert(name);
			}
		}
	}
	
	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("MapSelectDialogController::PropertyChanged");
		
		switch (property_name) {
			
			case "MapHostListboxSelection": {

				ViewBindingSet view_set = m_ViewBindingHashMap.Get("EditorDialogButton");
				m_ViewBindingHashMap.DebugPrint();
				if (view_set)
					foreach (ViewBinding view: view_set)
						if (view.GetRelayCommand())
							view.GetRelayCommand().SetCanExecute(MapHostListboxSelection != string.Empty);
					
				
				
				break;
			}
		}
	}
	/*
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{		
		if (button != 0) return false; 

		if (w.GetName() == "MapHostListbox") {	
			GetEditor().GetEditorHud().GetModal().Close();
			GetGame().PlayMission(CreateEditorMission(MapHostListboxSelection));
		}
		
		return false;
	}*/
}


class MapSelectDialog: EditorDialog
{		
	void MapSelectDialog(string title = "Select Map")
	{
		EditorLog.Trace("MapSelectDialog");

		AddContent("DayZEditor/gui/Layouts/dialogs/EditorMapSelector.layout");		
		SetTitle(title);
		AddButton("Select", "MapSelectDialogRelayCommand");
		AddButton("Close", "DialogCloseRelayCommand");
		MapSelectDialogController.Cast(GetController()).SetMapSelectDialog(this);
	}
	
	
	override typename GetControllerType() {
		return MapSelectDialogController;
	}
	

}