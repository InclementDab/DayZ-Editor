

class EditorMVCLayout: MVCLayout
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	protected EditorHudController m_EditorHudController;
	
	void EditorMVCLayout(EditorHudController controller = null)
	{
		EditorLog.Trace("EditorMVCLayout");
		m_Editor = GetEditor();
		if (m_Editor)
			m_EditorHud = m_Editor.GetEditorHud();
		
		m_EditorHudController = controller;
	}
	
}

