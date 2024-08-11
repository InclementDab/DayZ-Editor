class EditorMapSelectDialog: EditorDialogBase
{
	protected autoptr ListBoxPrefab<string> m_ListBoxPrefab;
	protected DialogButton m_SelectButton;
	
	void EditorMapSelectDialog(string title)
	{
		m_ListBoxPrefab = new ListBoxPrefab<string>();
		AddContent(m_ListBoxPrefab);

		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			string worldPath = string.Format("CfgWorlds %1", name);
			// Check if worldName exists in config to verify if this is the actuall world name child
			string worldNamePath = worldPath + " worldName";
			if (!GetGame().ConfigIsExisting(worldNamePath))
				continue;
			
			m_ListBoxPrefab.InsertItem(name, name);
		}
		
		AddContent(new TextBoxPrefab("NOTE: The Editor automatically detects custom map mods. Just load them.", null, "hint"));    
		
		m_SelectButton = AddButton("Select", DialogResult.OK);
		//m_SelectButton.Button.Enable(false);
		//m_SelectButton.Button.SetAlpha(150);
		
		AddButton(DialogResult.Cancel);
	}
	
	DialogResult ShowDialog(out string selected_map)
	{
		ListBoxPrefab<string> list_box = m_ListBoxPrefab; // ViewController is ref'd inside this
		DialogResult result = ShowDialog();
		selected_map = list_box.GetSelectedItem();
		delete m_ListBoxPrefab;
		return result;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (/*w == m_ListBoxPrefab.ListBox && */button == 0) {
			CloseDialog(DialogResult.OK);
			return true;
		}
		
		return super.OnDoubleClick(w, x, y, button);
	}
}