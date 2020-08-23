
class TypeConversion
{
	bool GetBool();
	float GetFloat();	
	string GetString();
	Widget GetWidget();
	
	void SetFromModel(Class context, string name, int index);
}

class TypeConversionTemplate<Class T>: TypeConversion
{
	protected T value;
	
	private void NotImplementedException(string method_name)
	{
		string msg = string.Format("NotImplementedException: %1", method_name);
		#ifdef COMPONENT_SYSTEM
		Error(msg);
		Workbench.Dialog("Error", msg);
		#else
		EditorLog.Error(msg);
		#endif
	}
	
	T Get(Class context, string name, int index)
	{
		T _data;
		EnScript.GetClassVar(context, name, index, _data);
		return _data;
	}
	
	override void SetFromModel(Class context, string name, int index)
	{
		EnScript.GetClassVar(context, name, index, value);
	}
	
	void Set(T _value)
	{
		value = _value;
	}
}

class TypeConversionBool: TypeConversionTemplate<bool>
{
	override bool GetBool()
	{
		return value;
	}
}

class TypeConversionFloat: TypeConversionTemplate<float>
{
	override float GetFloat()
	{
		return value;
	}
}

class TypeConversionString: TypeConversionTemplate<string>
{	
	override bool GetBool()
	{
		return string.ToString(value, false, false, false) == "1";
	}
	
	override float GetFloat()
	{
		return string.ToString(value, false, false, false).ToFloat();
	}
	
	override string GetString()
	{
		return string.ToString(value, false, false, false);
	}
}

class TypeConversionHashMap: ref map<typename, typename>
{
	void TypeConversionHashMap()
	{
		RegisterTypeConversion(bool, TypeConversionBool);
		RegisterTypeConversion(float, TypeConversionFloat);
		RegisterTypeConversion(string, TypeConversionString);
	}
	
	bool RegisterTypeConversion(typename conversion_type, typename conversion_class)
	{		
		if (conversion_class.IsInherited(TypeConversionTemplate)) {
			return Insert(conversion_type, conversion_class);
		} else {
			Error("RegisterTypeConversion: TypeConversionType must be of type TypeConversionTemplate");
		}
		
		return false;
	}
}

class TypeConverter
{
	private static ref TypeConversionHashMap m_TypeConversionHashMap = new TypeConversionHashMap();
	static typename GetTypeConversion(typename type) { return m_TypeConversionHashMap.Get(type); }
	
	Class Context;
	string Name;
	int Index;
	
	void TypeConverter(Class context, string name, int index) 
	{
		Context = context; Name = name; Index = index;
		
		if (m_TypeConversionHashMap == null) {
			m_TypeConversionHashMap = new TypeConversionHashMap();
		}
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
	
	void OnPropertyChanged(Controller controller)
	{
		EditorLog.Debug("ViewBinding::OnPropertyChanged");
		
		typename conversion_input = controller.GetPropertyType(Binding_Name);
		typename conversion_output = GetWidgetDataType();
				
		
		// If the property of the Controller is NOT the native widget data type			
		if (conversion_output != conversion_input) {
			EditorLog.Debug(string.Format("Attempting Type Conversion from %1 to %2", conversion_input, conversion_output));
		}
		

		
		TypeConverter type_converter = new TypeConverter(controller, Binding_Name, Binding_Index);
		EditorLog.Debug(string.Format("ConversionInput: %1, ConversionOutput: %2", conversion_input, conversion_output));
		
		// Sets data value into the converter (intermediate data)
		switch (conversion_input) {
			
			case bool: {
				//TypeConversionBool _TypeConversionBool = TypeConverter.GetTypeConversion(conversion_output).Spawn();
				//g_Script.Call(m_LayoutRoot, widget_setter, _TypeConversionBool.SetBool(_TypeConversionBool.Get(controller, Binding_Name, Binding_Index)));
				break;
			}
			
			case float: {
				//TypeConversionFloat _TypeConversionFloat = TypeConverter.GetTypeConversion(conversion_output).Spawn();
				//g_Script.Call(m_LayoutRoot, widget_setter, _TypeConversionFloat.SetFloat(_TypeConversionFloat.Get(controller, Binding_Name, Binding_Index)));
				break;
			}
			
			case string: {
				
				TypeConversionString _TypeConversionString = TypeConverter.GetTypeConversion(conversion_input).Spawn();
				_TypeConversionString.SetFromModel(controller, Binding_Name, Binding_Index);
				UpdateView(_TypeConversionString, conversion_output);
				break;
			}
			
			default: {
				UnsupportedConversionError(conversion_input, conversion_output);
			}
			
		}
	}
	
	void UpdateView(TypeConversion type_conversion, typename conversion_output)
	{
		EditorLog.Trace("UpdateView: " + type_conversion);
		
		string widget_setter = GetWidgetSetter(m_LayoutRoot.Type());
		if (widget_setter == string.Empty) {
			UnsupportedTypeError(m_LayoutRoot.Type());
			return;
		}
		
		switch (conversion_output)
		{
			
			case bool: {
				g_Script.Call(m_LayoutRoot, widget_setter, type_conversion.GetBool());
				break;
			}
			
			case float: {
				g_Script.Call(m_LayoutRoot, widget_setter, type_conversion.GetFloat());
				break;
			}
			
			case string: {
				g_Script.Call(m_LayoutRoot, widget_setter, type_conversion.GetString());
				break;
			}
			
			default: {
				UnsupportedConversionError(type_conversion.Type(), conversion_output);
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
	
	private TypeConverter GetTypeConverter(Controller controller)
	{
		
		switch (controller.GetPropertyType(Binding_Name)) {
			
			case bool: {
				return new TypeConverter<bool>(controller, Binding_Name, Binding_Index);
			}
			
			case float: {
				return new TypeConverter<float>(controller, Binding_Name, Binding_Index);
			}
			
			case string: {
				return new TypeConverter<string>(controller, Binding_Name, Binding_Index);
			}
			
			default: {
				Controller.ErrorDialog(string.Format("Unsupported data converter: %1", controller.GetPropertyType(Binding_Name)));
			}
		}
		
		return null;
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


