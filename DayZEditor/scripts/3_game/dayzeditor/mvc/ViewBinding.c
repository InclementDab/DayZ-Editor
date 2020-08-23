


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
	
	void OnPropertyChanged(Controller controller)
	{
		EditorLog.Trace("ViewBinding::OnPropertyChanged");
		
		typename conversion_input = controller.GetPropertyType(Binding_Name);
		typename conversion_output = GetWidgetDataType();
				
		
		// If the property of the Controller is NOT the native widget data type			
		if (conversion_output != conversion_input) {
			EditorLog.Debug(string.Format("Attempting Type Conversion from %1 to %2", conversion_input, conversion_output));
		}
				
		EditorLog.Debug(string.Format("ConversionInput: %1, ConversionOutput: %2", conversion_input, conversion_output));
		
		// Sets data value into the converter (intermediate data)
		TypeConverter _TypeConverter = TypeConverter.GetTypeConversion(conversion_input);
		_TypeConverter.SetFromController(controller, Binding_Name, Binding_Index);
		UpdateView(_TypeConverter, conversion_output);
	}
	
	void UpdateView(TypeConverter type_converter, typename conversion_output)
	{
		EditorLog.Trace("ViewBinding::UpdateView");

		string widget_setter = GetWidgetSetter(m_LayoutRoot.Type());
		if (widget_setter == string.Empty) {
			UnsupportedTypeError(m_LayoutRoot.Type());
			return;
		}
		
		switch (conversion_output)
		{
			case bool: {
				g_Script.Call(m_LayoutRoot, widget_setter, type_converter.GetBool());
				break;
			}
			
			case float: {
				g_Script.Call(m_LayoutRoot, widget_setter, type_converter.GetFloat());
				break;
			}
			
			case string: {
				g_Script.Call(m_LayoutRoot, widget_setter, type_converter.GetString());
				break;
			}
			
			default: {
				UnsupportedConversionError(type_converter.Type(), conversion_output);
			}
			
		}
	}
	
	
	void UnsupportedTypeError(typename type)
	{
		Controller.ErrorDialog(string.Format("%1: Unsupported Type %2", m_LayoutRoot.Type(), type));
	}
	
	void UnsupportedConversionError(typename from_type, typename to_type)
	{
		Controller.ErrorDialog(string.Format("Unsupported conversion from %1 to %2!", from_type, to_type));
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


