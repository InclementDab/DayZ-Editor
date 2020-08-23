


class ViewBinding: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	protected reference string Binding_Name;
	protected reference int Binding_Index;
	
	Widget GetRoot() { return m_LayoutRoot; }
	string GetBindingName() { return Binding_Name; }
	int GetBindingIndex() { return Binding_Index; }

	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("ViewBinding::Init");
		m_LayoutRoot = w;
		
		if (Binding_Name == string.Empty) {
			Binding_Name = m_LayoutRoot.GetName();
		}
	}
	
	void OnPropertyChanged(WidgetData converter)
	{
		Print(converter);
		WidgetDataConverter<Class> conv = converter;
		Print(conv);
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("ViewBinding::OnChange");
		return super.OnChange(w, x, y, finished);
	}
}


