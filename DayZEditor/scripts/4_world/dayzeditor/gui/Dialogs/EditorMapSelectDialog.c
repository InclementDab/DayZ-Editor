

static const ref array<string> ExcludedMapItems = {
	"access",
	"DefaultLighting",
	"DefaultWorld",
	"initWorld",
	"CAWorld"
};


class MapSelectDialogController: Controller
{
	TextListboxWidget MapHostListbox;
	
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (ExcludedMapItems.Find(name) == -1)
				MapHostListbox.AddItem(name, null, 0);
		}
	}
}


class MapSelectDialog: EditorDialog
{	
		
	private ref ButtonWidget m_SelectButton;
	private ref ButtonWidget m_CloseButton;
	
	protected ref MapSelectDialogController m_MapSelectDialogController;
		
	void MapSelectDialog(string title = "Select Map")
	{
		EditorLog.Trace("MapSelectDialog");
		m_MapSelectDialogController = AddContent("DayZEditor/gui/Layouts/dialogs/EditorMapSelector.layout");
		m_SelectButton = AddButton("Select", "SelectCallback");
		m_CloseButton = AddButton("Close", "CloseCallback");
	}
	
	
	override void Update()
	{
		bool select_state = m_MapSelectDialogController.MapHostListbox.GetSelectedRow() != -1;
		if (select_state) {
			m_SelectButton.SetAlpha(1);
		}
		else {
			m_SelectButton.SetAlpha(0.3); 
		}
		
		m_SelectButton.Enable(select_state);
	}
	
	void SelectCallback()
	{
		EditorLog.Trace("MapSelectDialog::SelectCallback");
		
		string name;
		m_MapSelectDialogController.MapHostListbox.GetItemText(m_MapSelectDialogController.MapHostListbox.GetSelectedRow(), 0, name);
		GetGame().PlayMission(CreateEditorMission(name));
		CloseDialog();
	}
	
	void CloseCallback()
	{
		EditorLog.Trace("MapSelectDialog::CloseCallback");
		CloseDialog();
	}
	
	
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{		
		if (button != 0) return false; 

		if (w == m_MapSelectDialogController.MapHostListbox) {
			string name;
			m_MapSelectDialogController.MapHostListbox.GetItemText(m_MapSelectDialogController.MapHostListbox.GetSelectedRow(), 0, name);
			GetGame().PlayMission(CreateEditorMission(name));
			CloseDialog();
			return true;
			
		}		
		return false;
	}
	

}