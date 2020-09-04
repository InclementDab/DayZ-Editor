class EditorPrefabViewBinding: ViewBinding
{
	
	protected ref EditorPrefab m_EditorPrefab;
	
	void SetPrefab(EditorPrefab prefab) {
		m_EditorPrefab = prefab;
		SetBindingName(m_EditorPrefab.GetBindingName());
		
		m_WidgetController = MVC.GetWidgetController(m_EditorPrefab);
	}
	
	
	
	private bool group_state = true;
	override bool OnClick(Widget w, int x, int y, int button)
	{
		
		switch (w.GetName()) {
			
			case "EditorDialogOptionButton": {
				group_state = !group_state;
				w.FindAnyWidget(string.Format("%1_expand_image", w.GetName())).Show(group_state);
				w.FindAnyWidget(string.Format("%1_collapse_image", w.GetName())).Show(!group_state);
				w.GetSibling().Show(group_state);
				return true;
			}
			
		}
		
		return false;
	}
	
}