


class DataBinding: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	
	[Attribute("", "script")]
	string test1;
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("DataBinding::Init");
		m_LayoutRoot = w;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("DataBinding::OnChange");
		
		return super.OnChange(w, x, y, finished);
	}
	
	
	
}

