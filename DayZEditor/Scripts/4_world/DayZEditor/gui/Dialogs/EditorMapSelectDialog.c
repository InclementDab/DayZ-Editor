static const ref array<string> ExcludedMapItems = {
	"access",
	"DefaultLighting",
	"DefaultWorld",
	"initWorld",
	"CAWorld"
};

class EditorMapSelectDialog: EditorDialogBase
{
	protected autoptr ListBoxPrefab m_ListBoxPrefab;
	
	void EditorMapSelectDialog(Widget parent = null, string title = "")
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
		
		AddButton("Select", DialogResult.OK);
		AddButton("Cancel", DialogResult.Cancel);
	}
	
	DialogResult ShowDialog(out string selected_map)
	{
		ListBoxPrefabController controller = m_ListBoxPrefab.GetListBoxPrefabController();
		DialogResult result = ShowDialog();
		selected_map = controller.SelectedListBoxItem;
		return result;
	}
}