static const ref array<string> ExcludedMapItems = 
{
	"access",
	"DefaultLighting",
	"DefaultWorld",
	"initWorld",
	"CAWorld"
};

class EditorMapSelectDialog: EditorDialogBase
{
	protected autoptr ListBoxPrefab m_ListBoxPrefab;
	protected DialogButton m_SelectButton;
	
	void EditorMapSelectDialog(string title)
	{
		m_ListBoxPrefab = new ListBoxPrefab();
		AddContent(m_ListBoxPrefab);
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (ExcludedMapItems.Find(name) == -1) {
				m_ListBoxPrefab.GetListBoxPrefabController().ListBoxData.Insert(name);
			}
		}
		
		m_SelectButton = AddButton("Select", DialogResult.OK);
		//m_SelectButton.Button.Enable(false);
		//m_SelectButton.Button.SetAlpha(150);
		
		AddButton("Cancel", DialogResult.Cancel);
	}
	
	DialogResult ShowDialog(out string selected_map)
	{
		ListBoxPrefab list_box = m_ListBoxPrefab; // controller is ref'd inside this
		DialogResult result = ShowDialog();
		selected_map = list_box.GetListBoxPrefabController().SelectedListBoxItem;
		delete m_ListBoxPrefab;
		return result;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (w == m_ListBoxPrefab.ListBox && button == 0) {
			CloseDialog(DialogResult.OK);
			return true;
		}
		
		return super.OnDoubleClick(w, x, y, button);
	}
}