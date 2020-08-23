
class WidgetDataConverter
{
	bool ToBool()
	{
		return true;
	}
	
	float ToFloat()
	{
		return 0;
	}
}


class WidgetData<Class T>
{
	static void Set(Widget w, T data)
	{
		EditorLog.Trace("WidgetData::Set");
		typename widget_type = DataBindingBase.GetWidgetDataType(w.Type());
		Print(widget_type);
		switch (widget_type) {
			
			case bool: {
				switch (w.Type()) {
					
					case ButtonWidget: {
						ButtonWidget.Cast(w).SetState(data);
						break;
					}
				}
				break;
			}
			
			case float: {
				switch (w.Type()) {
					
					case SliderWidget: {
						SliderWidget.Cast(w).SetCurrent(data);
						break;
					}
				}
			}
			
		}
	}
	
	static T Get(Widget w)
	{		
		T result;
		switch (w.Type()) {
			
			case ButtonWidget: {
				return ButtonWidget.Cast(w).GetState();
			}
			
			case SliderWidget: {
				return SliderWidget.Cast(w).GetCurrent();
			}
			
		}
		
		return result;
	}
}


class ViewBinding: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	protected reference string Binding_Name;
	protected reference int Binding_Index;
	protected reference bool Two_Way_Binding;
	
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
		
		if (Two_Way_Binding && !DataBindingBase.SupportsTwoWayBinding(m_LayoutRoot.Type())) {
			Controller.ErrorDialog(string.Format("Two Way Binding for %1 is not supported!", m_LayoutRoot.Type()));
		}		
	}
	
	void UnsupportedTypeError(typename type)
	{
		Controller.ErrorDialog(string.Format("%1: Unsupported Type %2", m_LayoutRoot.Type(), type));
	}
	
	
	
	void SetData(WidgetDataConverter converter)
	{
		EditorLog.Trace("ViewBinding::SetData");
		typename widget_type = DataBindingBase.GetWidgetDataType(m_LayoutRoot.Type());
		switch (widget_type) {
			
			case bool: {
				WidgetData<bool>.Set(m_LayoutRoot, converter.ToBool());
				break;
			}
			
			case float: {
				WidgetData<float>.Set(m_LayoutRoot, converter.ToFloat());
				break;
			}
			
			default: {
				UnsupportedTypeError(widget_type);
			}
		}
	}
	
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("ViewBinding::OnChange");
		
		
		
		return super.OnChange(w, x, y, finished);
	}
}


