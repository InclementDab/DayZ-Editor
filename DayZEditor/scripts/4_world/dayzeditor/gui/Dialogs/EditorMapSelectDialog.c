

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
				Print(MapHostListboxSelection);
				break;
			}
		}
	}
	
	void SelectExecute()
	{
		EditorLog.Trace("MapSelectDialog::SelectExecute");
		
		string name;
		//m_MapSelectDialogController.MapHostListbox.GetItemText(m_MapSelectDialogController.MapHostListbox.GetSelectedRow(), 0, name);
		GetGame().PlayMission(CreateEditorMission(MapHostListboxSelection));
		GetEditor().GetEditorHud().GetModal().Close();
	}
}


class MapSelectDialog: EditorDialog
{		
		
	void MapSelectDialog(string title = "Select Map")
	{
		EditorLog.Trace("MapSelectDialog");

		AddContent("DayZEditor/gui/Layouts/dialogs/EditorMapSelector.layout");		
		SetTitle(title);
		AddButton("Select", "SelectExecute");
		AddButton("Close", "DialogCloseRelayCommand");
		MapSelectDialogController.Cast(GetController()).SetMapSelectDialog(this);
	}
	
	/*
	override void Update()
	{
		bool select_state = m_MapSelectDialogController.MapHostListbox.GetSelectedRow() != -1;
		if (select_state) {
			m_SelectButton.SetAlpha(1);
		}
		else m_SelectButton.SetAlpha(0.3); 
		
		
		m_SelectButton.Enable(select_state);
	}*/
	


	
	/*
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{		
		if (button != 0) return false; 

		if (w == m_MapSelectDialogController.MapHostListbox) {
			string name;
			m_MapSelectDialogController.MapHostListbox.GetItemText(m_MapSelectDialogController.MapHostListbox.GetSelectedRow(), 0, name);
			GetGame().PlayMission(CreateEditorMission(name));
			Close();
			return true;
			
		}		
		return false;
	}*/
	
	override typename GetControllerType() {
		return MapSelectDialogController;
	}
	

}