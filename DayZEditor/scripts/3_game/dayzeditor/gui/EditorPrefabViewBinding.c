class EditorPrefabViewBinding: ViewBinding
{
	
	protected ref EditorPrefab m_EditorPrefab;
	
	
	
	void SetPrefab(EditorPrefab prefab) {
		m_EditorPrefab = prefab;
		SetBindingName(m_EditorPrefab.GetBindingName());
		
		m_WidgetController = MVC.GetWidgetController(m_EditorPrefab);
	}
}