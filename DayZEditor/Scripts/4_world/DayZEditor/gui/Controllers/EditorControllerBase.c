class EditorControllerBase: Controller
{
	protected Editor m_Editor;
	
	void EditorControllerBase()
	{
		m_Editor = GetEditor();
	}
}