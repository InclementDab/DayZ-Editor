

static const ref array<string> ExcludedMapItems = {
	"access",
	"DefaultLighting",
	"DefaultWorld",
	"initWorld",
	"CAWorld"
};


class MapSelectDialog: EditorDialog
{	
	
	protected TextListboxWidget m_MapHostListbox;
	
	private ref ButtonWidget m_SelectButton;
	private ref ButtonWidget m_CloseButton;
		
	void MapSelectDialog()
	{
		Widget content = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorMapSelector.layout", null);
		SetContent(content);
		SetTitleText("Select Map...");
		m_SelectButton = AddButton("Select");
		m_CloseButton = AddButton("Close");
		
		m_MapHostListbox = TextListboxWidget.Cast(m_Root.FindAnyWidget("MapHostListbox"));
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (ExcludedMapItems.Find(name) == -1) {
				m_MapHostListbox.AddItem(name, null, 0);
			}
		}
		
	}
	
	
	
	override void Update()
	{
		m_TitleBar.SetPos(0, 0);
		bool lots_of_branches_on_your_penis = m_MapHostListbox.GetSelectedRow() != -1;
		if (lots_of_branches_on_your_penis)
			m_SelectButton.SetAlpha(1);
		else m_SelectButton.SetAlpha(0.3); 
		m_SelectButton.Enable(lots_of_branches_on_your_penis);
	}
	



	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("MapSelectDialog::OnClick");
		
		if (button != 0) return false; 
		
		if (w == m_SelectButton) {
			string name;
			m_MapHostListbox.GetItemText(m_MapHostListbox.GetSelectedRow(), 0, name);
			GetGame().PlayMission(CreateEditorMission(name));
			CloseDialog();
			return true;
		} 
		
		if (w == m_CloseButton) {
			CloseDialog();
			return true;
		}
	
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnDoubleClick( Widget w, int x, int y, int button )
	{		
		if (button != 0) return false; 
		Print(w);

		if (w == m_MapHostListbox) {
			string name;
			m_MapHostListbox.GetItemText(m_MapHostListbox.GetSelectedRow(), 0, name);
			GetGame().PlayMission(CreateEditorMission(name));
			CloseDialog();
			return true;
			
		}
		
		
		return super.OnDoubleClick( w, x, y, button );
	}
	

}