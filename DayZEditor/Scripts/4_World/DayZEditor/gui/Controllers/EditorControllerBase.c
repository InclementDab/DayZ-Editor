class EditorControllerBase: Controller
{
	protected Editor m_Editor;
	
	void EditorControllerBase()
	{
		m_Editor = GetEditor();
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		m_Editor = GetEditor();
		super.OnWidgetScriptInit(w);
	}
}