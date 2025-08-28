class ScriptWidgetBase: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		if (m_LayoutRoot) {
			ScriptView.LoadWidgetsAsVariables(this, w);
		}
		
		if (g_Game) {
			g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		}
	}
	
	protected void Update(float timeslice)
	{
	}
}