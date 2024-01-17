class EditorCategory: EditorSelectableBase
{
	protected ref array<EditorSelectableBase> m_Children = {};
	
	void EditorCategory(string name)
	{
		m_DisplayName = name;
	}
}