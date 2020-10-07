
// idea for putting the translationwidget into an itempreview. make it work
class TranslationWidgetHandler: ScriptedWidgetEventHandler
{
	protected ItemPreviewWidget m_LayoutRoot;
	protected EntityAI m_WidgetObject;
	
	void TranslationWidgetHandler()
	{
		EditorLog.Trace("TranslationWidgetHandler");
	}
	
	void ~TranslationWidgetHandler()
	{
		EditorLog.Trace("~TranslationWidgetHandler");
	}
	
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("TranslationWidgetHandler::OnWidgetScriptInit");
		m_LayoutRoot = ItemPreviewWidget.Cast(w);
		m_LayoutRoot.SetHandler(this);
		
		m_LayoutRoot.SetItem(EntityAI.Cast(GetGame().CreateObjectEx("TranslationWidget", vector.Zero, ECE_NONE, RF_FRONT)));
		m_LayoutRoot.SetView(0);
		m_LayoutRoot.Update();
	}
}





