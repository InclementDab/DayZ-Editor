
static ref ScriptInvoker OnMVCEventHandlerCreated = new ScriptInvoker();
static void MVCEventHandlerCreated(MVCEventHandler event_handler)
{
	EditorLog.Trace("EditorEvents::ViewBindingCreated");
	OnMVCEventHandlerCreated.Invoke(event_handler);
}

class MVCEventHandler: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot()
		return m_LayoutRoot;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
		
		MVCEventHandlerCreated(this);
	}
	
	
	void MVCOnMouseDown(Widget target, int button, int x, int y);
	void MVCOnMouseUp(Widget target, int button, int x, int y);
	void MVCOnMouseWheel(Widget target, int direction, int x, int y);
	void MVCOnClick(Widget target, int button, int x, int y);
	void MVCOnDoubleClick(Widget target, int button, int x, int y);
	void MVCOnKeyPress(Widget target, int key);
	void MVCOnMouseEnter(Widget target, int x, int y);
	void MVCOnMouseLeave(Widget target, int x, int y);
	
	void MVCOnDrag(Widget target, int x, int y);
	void MVCOnDrop(Widget target, Widget drop_target, int x, int y);
	void MVCOnDragging(Widget target, int x, int y);
	void MVCOnDropReceived(Widget target, Widget received_target, int x, int y);
	
}

