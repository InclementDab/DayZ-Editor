class EditorDialogButtonHandler: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	protected Widget DialogButtonOutline;
	protected ButtonWidget DialogButton;
	
	protected Class m_CallbackContext;
	protected string m_CallbackAction;

	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorDialogButtonHandler::OnWidgetScriptInit");
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	}
	
	void SetLabel(string label) {
		DialogButton.SetText(label);
	}
	
	void SetCallback(Class context, string action) {
		m_CallbackContext = context;
		m_CallbackAction = action;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{	
		EditorLog.Trace("EditorDialogButtonHandler::OnClick");
		if (button != 0) return false;
		
		if (m_CallbackContext && m_CallbackAction) {
			g_Script.Call(m_CallbackContext, m_CallbackAction, null);
			return true;
		}
				
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorDialogButtonHandler::OnMouseEnter");
		
		
		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		EditorLog.Trace("EditorDialogButtonHandler::OnMouseLeave");
		
		
		return false;
	}
	
	
	
}