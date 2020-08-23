

class WidgetDataConverter<Class T>
{
	static bool ToBool(T data)
	{
		string str = string.ToString(data);
		str.ToLower();
		return str == "true";
	}
	
	static float ToFloat(T data)
	{	
		string str = string.ToString(data);
		return str.ToFloat();
	}
	
	static string _ToString(T data)
	{
		return string.ToString(data);
	}
}



class WidgetData
{
	static typename GetWidgetDataType(typename widget_type)
	{
		switch (widget_type) {
			
			case Widget:
			case SpacerBaseWidget:
				return Widget;
			
			case ButtonWidget:
			case CheckBoxWidget:
				return bool;
			
			case SliderWidget:
			case ProgressBarWidget:
			case SimpleProgressBarWidget:
				return float;
			
			case TextWidget:
			case ImageWidget:
			case EditBoxWidget:
			case HtmlWidget:
			case VideoWidget:
				return string;
			
			case RichTextWidget:
			case MultilineTextWidget:
			case MultilineEditBoxWidget:
			case XComboBoxWidget:
				return TStringArray;
			
			case ItemPreviewWidget:
				return EntityAI;
			
			case PlayerPreviewWidget:
				return DayZPlayer;
			
			default: {
				Error(string.Format("Unknown Type Specified %1", widget_type));
			}
		}
		
		return typename;
	}
	
	static bool SupportsTwoWayBinding(typename type)
	{
		switch (type) {
			case ButtonWidget:
			case CheckBoxWidget:
			case SliderWidget:
			case EditBoxWidget:
			case MultilineEditBoxWidget:
			case RichTextWidget:
				return true;
	
		}
		
		return false;
	}
	
	static void Set(Widget w, bool data)
	{
		EditorLog.Trace("WidgetData::Set::Bool");
		
		switch (w.Type()) {
			
			case ButtonWidget: {
				ButtonWidget.Cast(w).SetState(data);
				break;
			}
		}
	}
	
	static void Set(Widget w, float data)
	{
		switch (w.Type()) {
			case SliderWidget: {
				SliderWidget.Cast(w).SetCurrent(data);
				break;
			}
		}
	}
	
	static void Set(Widget w, string data)
	{
		switch (w.Type()) {
			case TextWidget: {
				TextWidget.Cast(w).SetText(data);
				break;
			}
		}
	}
	/*
	static T Get(Widget w)
	{
		switch (w.Type()) {
			
			case ButtonWidget: {
				return ButtonWidget.Cast(w).GetState();
			}
			
			case SliderWidget: {
				return SliderWidget.Cast(w).GetCurrent();
			}

		}
		
		T result;
		
		Class.CastTo(result, 0);
		return result;
	}*/
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
		
		if (Two_Way_Binding && !WidgetData.SupportsTwoWayBinding(m_LayoutRoot.Type())) {
			Controller.ErrorDialog(string.Format("Two Way Binding for %1 is not supported!", m_LayoutRoot.Type()));
		}		
	}
	

	
	

	
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("ViewBinding::OnChange");
		
		
		
		return super.OnChange(w, x, y, finished);
	}
}


