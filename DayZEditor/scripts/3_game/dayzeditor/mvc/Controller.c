

class Controller: Managed
{
	protected Widget m_LayoutRoot;
	
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("Controller::Init");
		
		m_LayoutRoot = w;
	}	
}

