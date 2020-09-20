

class EditorMVCLayout: MVCLayout
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	void EditorMVCLayout(Widget parent = null)
	{
		EditorLog.Trace("EditorMVCLayout");
		m_Editor = GetEditor();
		if (m_Editor) {
			m_EditorHud = m_Editor.GetEditorHud();	
		}
			
	}
	
	void ~EditorMVCLayout()
	{
		EditorLog.Trace("~EditorMVCLayout");
	}
	
	void SetPosition(float x, float y) {
		m_LayoutRoot.SetPos(x, y);
	}
}

