


class ViewBinding: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	protected reference string Binding_Name;
	protected reference int Binding_Index;
	protected reference bool Two_Way_Binding;
	
	Widget GetRoot() { return m_LayoutRoot; }
	string GetBindingName() { return Binding_Name; }
	int GetBindingIndex() { return Binding_Index; }
	
	private ref TypeConverter m_InputTypeConverter;
	private ref TypeConverter m_OutputTypeConverter;
	
	private typename m_ConversionInput;
	private typename m_ConversionOutput;
	
	private Controller m_Controller;
	void SetController(Controller controller) 
	{ 
		EditorLog.Trace("ViewBinding::SetController");
		m_Controller = controller;
		
		m_ConversionInput = m_Controller.GetPropertyType(Binding_Name);
		m_ConversionOutput = GetWidgetDataType();
		
		m_InputTypeConverter = MVC.GetTypeConversion(m_ConversionInput);
		m_OutputTypeConverter = MVC.GetTypeConversion(m_ConversionOutput);
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("ViewBinding::Init");
		m_LayoutRoot = w;
		
		if (Binding_Name == string.Empty) {
			Binding_Name = m_LayoutRoot.GetName();
		}
		
		if (Two_Way_Binding && !ViewBinding.SupportsTwoWayBinding(m_LayoutRoot.Type())) {
			MVC.ErrorDialog(string.Format("Two Way Binding for %1 is not supported!", m_LayoutRoot.Type()));
		}
		
		m_LayoutRoot.SetHandler(this);
	}
	
	void OnPropertyChanged()
	{
		EditorLog.Trace("ViewBinding::OnPropertyChanged");
				
		// If the property of the Controller is NOT the native widget data type			
		if (m_ConversionOutput != m_ConversionInput) {
			EditorLog.Debug(string.Format("Attempting Type Conversion from %1 to %2", m_ConversionInput, m_ConversionOutput));
		}
				
		EditorLog.Debug(string.Format("ConversionInput: %1, ConversionOutput: %2", m_ConversionInput, m_ConversionOutput));
		
		// Sets data value into the converter (intermediate data)
		
		if (!m_InputTypeConverter) {
			MVC.ErrorDialog(string.Format("Could not find TypeConversion for Type %1\nUse TypeConverter.RegisterTypeConversion to register custom types", m_ConversionInput));
			return;
		}
		
		m_InputTypeConverter.GetFromController(m_Controller, Binding_Name, Binding_Index);
		UpdateView();
		m_Controller.PropertyChanged(Binding_Name);
	}
	
	private void UpdateView()
	{
		EditorLog.Trace("ViewBinding::UpdateView");
		string widget_setter = GetWidgetSetter(m_LayoutRoot.Type());
		if (widget_setter == string.Empty) {
			MVC.UnsupportedTypeError(m_LayoutRoot.Type());
			return;
		}
		
		switch (m_ConversionOutput)
		{
			case bool: {
				g_Script.Call(m_LayoutRoot, widget_setter, m_InputTypeConverter.GetBool());
				break;
			}
			
			case float: {
				g_Script.Call(m_LayoutRoot, widget_setter, m_InputTypeConverter.GetFloat());
				break;
			}
			
			case string: {
				g_Script.Call(m_LayoutRoot, widget_setter, m_InputTypeConverter.GetString());
				break;
			}
			
			case Widget: {
				g_Script.Call(m_LayoutRoot, widget_setter, m_InputTypeConverter.GetWidget());
				break;
			}
			
			default: {
				MVC.UnsupportedConversionError(m_InputTypeConverter.Type(), m_ConversionOutput);
			}
			
		}
	}
		
	private void UpdateModel()
	{
		if (!Two_Way_Binding || !SupportsTwoWayBinding(m_LayoutRoot.Type())) return;
		EditorLog.Trace("ViewBinding::UpdateModel");
		
		string widget_getter = GetWidgetGetter(m_LayoutRoot.Type());
		if (widget_getter == string.Empty) {
			MVC.UnsupportedTypeError(m_LayoutRoot.Type());
			return;
		}
		
		switch (m_ConversionInput) {
			
			case bool: {
				bool _bool;
				g_Script.CallFunction(m_LayoutRoot, widget_getter, _bool, null);
				m_OutputTypeConverter.SetBool(_bool);
				break;
			}
			
			case float: {
				float _float;
				g_Script.CallFunction(m_LayoutRoot, widget_getter, _float, null);
				m_OutputTypeConverter.SetFloat(_float);
				break;
			}			
			
			case string: {
				string _string;
				g_Script.CallFunction(m_LayoutRoot, widget_getter, _string, null);
				m_OutputTypeConverter.SetString(_string);
				break;
			}			
			
			case Widget: {
				Widget _widget;
				g_Script.CallFunction(m_LayoutRoot, widget_getter, _widget, null);
				m_OutputTypeConverter.SetWidget(_widget);
				break;
			}
			
			
			default: {
				MVC.UnsupportedConversionError(m_OutputTypeConverter.Type(), m_ConversionInput);
				return;
			}
		}
		
		if (m_OutputTypeConverter != null) { 
			m_OutputTypeConverter.SetToController(m_Controller, Binding_Name, Binding_Index);
			m_Controller.PropertyChanged(Binding_Name);
		}
	}
	
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("ViewBinding::OnChange");
		
		UpdateModel();
		
		
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
	
	static string GetWidgetGetter(typename widget_type)
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
				return "GetState";
			
			case CheckBoxWidget:
				return "IsChecked";
			
			case SliderWidget:			
			case ProgressBarWidget:
			case SimpleProgressBarWidget:
				return "GetCurrent";
			
			case EditBoxWidget:
				return "GetText";
			
			
			/* Unsupported
			case ImageWidget:
			case VideoWidget:
				return string.Empty;*/
			
			/* Observables
			case XComboBoxWidget:
			case MultilineTextWidget:
				return TStringArray;*/
			
			case ItemPreviewWidget:
				return "GetItem";
			
			case PlayerPreviewWidget:
				return "GetDummyPlayer";
			
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
}


