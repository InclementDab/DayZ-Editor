class EditorWidgetEventHandler: ScriptedWidgetEventHandler
{
	protected ref Widget m_Root;

	void ~EditorWidgetEventHandler()
	{
		m_Root.Show(false);
	}
	
	
	
	void OnWidgetScriptInit(Widget w)
	{
		Print("EditorWidgetEventHandler::Init");
		m_Root = w;
		m_Root.SetHandler(this);
	}
	
	
	Widget GetRoot() { return m_Root; }
	void Update(float timeslice) { }
}
