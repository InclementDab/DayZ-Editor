
class TypeConverterBase
{
	bool GetBool() { return false; }
	float GetFloat() { return 0; }
	string GetString() { return string.Empty; }
}


class TypeConverter<Class T>: TypeConverterBase
{
	Class Context;
	string Name;
	int Index;
	
	void TypeConverter(Class context, string name, int index) 
	{
		Context = context; Name = name; Index = index;
	}
	
	private T Get()
	{
		T _data;
		EnScript.GetClassVar(Context, Name, Index, _data);
		return _data;
	}
	
	override bool GetBool()
	{
		return GetBool(Get());
	}
	
	override float GetFloat()
	{
		return GetFloat(Get());
	}
	
	override string GetString()
	{
		return GetString(Get());
	}
	
	
	static bool GetBool(T data)
	{
		return (string.ToString(data, false, false, false) == "1");
	}

	static float GetFloat(T data)
	{	
		return string.ToString(data, false, false, false).ToFloat();
	}

	static string GetString(T data)
	{
		return string.ToString(data, false, false, false);
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
		
		if (Two_Way_Binding && !ViewBinding.SupportsTwoWayBinding(m_LayoutRoot.Type())) {
			Controller.ErrorDialog(string.Format("Two Way Binding for %1 is not supported!", m_LayoutRoot.Type()));
		}
	}
	
	void Set(bool data)
	{
		EditorLog.Trace("ViewBinding::Set::Bool");

		string setter = GetWidgetSetter(m_LayoutRoot.Type());
		if (setter != string.Empty)
			g_Script.Call(m_LayoutRoot, setter, data);
		else UnsupportedTypeError(m_LayoutRoot.Type());
		
	}
	
	void Set(float data)
	{
		EditorLog.Trace("ViewBinding::Set::Float");
		
		
		string setter = GetWidgetSetter(m_LayoutRoot.Type());
		if (setter != string.Empty)
			g_Script.Call(m_LayoutRoot, setter, data);
		else UnsupportedTypeError(m_LayoutRoot.Type());
	}
	
	void Set(string data)
	{
		EditorLog.Trace("ViewBinding::Set::String");
				
		string setter = GetWidgetSetter(m_LayoutRoot.Type());
		if (setter != string.Empty)
			g_Script.Call(m_LayoutRoot, setter, data);
		else UnsupportedTypeError(m_LayoutRoot.Type());
	}
	
	void UnsupportedTypeError(typename type)
	{
		Controller.ErrorDialog(string.Format("%1: Unsupported Type %2", m_LayoutRoot.Type(), type));
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("ViewBinding::OnChange");
		return super.OnChange(w, x, y, finished);
	}
	
	static string GetWidgetSetter(typename widget_type)
	{
		switch (widget_type) {
			
			/* Observables
			case Widget:
			case SpacerBaseWidget:
			case GridSpacerWidget:
			case WrapSpacerWidget:
			case ScrollWidget:
			case SpacerWidget:
				return "AddChild";*/
			
			case ButtonWidget:
				return "SetState";
			
			case CheckBoxWidget:
				return "SetChecked";
			
			case SliderWidget:			
			case ProgressBarWidget:
			case SimpleProgressBarWidget:
				return "SetCurrent";
			
			case TextWidget:
			case EditBoxWidget:
			case RichTextWidget:
			case MultilineEditBoxWidget:
				return "SetText";
			
			case HtmlWidget:
				return "LoadFile";
			
			/* Unsupported
			case ImageWidget:
			case VideoWidget:
				return string.Empty;*/
			
			/* Observables
			case XComboBoxWidget:
			case MultilineTextWidget:
				return TStringArray;*/
			
			case ItemPreviewWidget:
				return "SetItem";
			
			case PlayerPreviewWidget:
				return "SetPlayer";
			
			default: {
				Error(string.Format("Unknown Type Specified %1", widget_type));
			}
		}
		
		return string.Empty;
	}
	

	typename GetWidgetDataType() { return GetWidgetDataType(GetRoot().Type()); }
	
	static typename GetWidgetDataType(typename widget_type)
	{
		switch (widget_type) {
			
			case Widget:
			case SpacerBaseWidget:
			case GridSpacerWidget:
			case WrapSpacerWidget:
			case ScrollWidget:
			case SpacerWidget:
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


