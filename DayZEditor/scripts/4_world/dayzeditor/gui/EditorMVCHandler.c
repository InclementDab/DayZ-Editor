

class EditorMVCLayout: MVCLayout
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	void EditorMVCLayout()
	{
		EditorLog.Trace("EditorMVCLayout");
		m_Editor = GetEditor();
		if (m_Editor)
			m_EditorHud = m_Editor.GetEditorHud();
	}
	
}

