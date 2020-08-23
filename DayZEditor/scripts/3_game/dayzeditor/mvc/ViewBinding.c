


class ViewBinding: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	protected reference string Binding_Name;
	protected reference int Binding_Index;
	protected reference bool Two_Way_Binding;
	
	Widget GetRoot() { return m_LayoutRoot; }
	string GetBindingName() { return Binding_Name; }
	int GetBindingIndex() { return Binding_Index; }
	
	private typename m_WidgetDataType;
	
	private ref TypeConverter m_PropertyDataConverter;
	private typename m_PropertyDataType;
	
	private Controller m_Controller;
	
	void ViewBinding() { EditorLog.Trace("ViewBinding"); }
	void ~ViewBinding() { EditorLog.Trace("~ViewBinding"); }
	
	void SetController(Controller controller) 
	{ 
		EditorLog.Trace("ViewBinding::SetController");
		m_Controller = controller;
		
		m_PropertyDataType = m_Controller.GetPropertyType(Binding_Name);		
		m_PropertyDataConverter = MVC.GetTypeConversion(m_PropertyDataType);
		m_WidgetDataType = GetWidgetDataType(m_LayoutRoot.Type());
		
		// Updates the view on first load
		if (m_PropertyDataConverter) {
			OnPropertyChanged();
		} else {
			EditorLog.Warning(string.Format("[%1] Binding not found!", m_LayoutRoot.GetName()));
		}
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
		EditorLog.Trace("ViewBinding::OnPropertyChanged " + Binding_Name);		
		//EditorLog.Debug(string.Format("PropertyDataType: %1, WidgetDataType: %2", m_PropertyDataType, m_WidgetDataType));
				
		if (!m_PropertyDataType) {
			MVC.ErrorDialog(string.Format("Binding not found: %1", Binding_Name));
			return;
		}
		
		if (!m_PropertyDataConverter) {
			MVC.ErrorDialog(string.Format("Could not find TypeConversion for Type %1\nUse TypeConverter.RegisterTypeConversion to register custom types", m_PropertyDataType));
			return;			
		}
		
		// If the property of the Controller is NOT the native widget data type			
		if (m_WidgetDataType != m_PropertyDataType) {
			EditorLog.Debug(string.Format("Attempting Type Conversion from %1 to %2", m_PropertyDataType, m_WidgetDataType));
		}
		
		m_PropertyDataConverter.GetFromController(m_Controller, Binding_Name, Binding_Index);
		
		
		string widget_setter = GetWidgetSetter(m_LayoutRoot.Type());
		if (widget_setter == string.Empty) {
			MVC.UnsupportedTypeError(m_LayoutRoot.Type());
			return;
		}
		
		EditorLog.Debug(string.Format("[%1] Updating View...", m_LayoutRoot.Type()));
		switch (m_WidgetDataType)
		{
			case bool: {
				g_Script.CallFunction(m_LayoutRoot, widget_setter, null, m_PropertyDataConverter.GetBool());
				break;
			}
			
			case float: {
				g_Script.CallFunction(m_LayoutRoot, widget_setter, null, m_PropertyDataConverter.GetFloat());
				break;
			}
			
			case string: {
				g_Script.CallFunction(m_LayoutRoot, widget_setter, null, m_PropertyDataConverter.GetString());
				break;
			}
						
			default: {
				MVC.UnsupportedConversionError(m_PropertyDataConverter.Type(), m_WidgetDataType);
				return;
			}
		}		
	}
	
	
	
	void UpdateModel()
	{
		if (!Two_Way_Binding || !SupportsTwoWayBinding(m_LayoutRoot.Type())) 
			return;
		
		EditorLog.Trace("ViewBinding::UpdateModel");
		EditorLog.Debug(string.Format("[%1] Updating Model...", m_LayoutRoot.Type()));
		
		string widget_getter = GetWidgetGetter(m_LayoutRoot.Type());
		if (widget_getter == string.Empty) {
			MVC.UnsupportedTypeError(m_LayoutRoot.Type());
			return;
		}
				
		switch (m_WidgetDataType) {
			
			case bool: {
				bool _bool;
				g_Script.CallFunction(m_LayoutRoot, widget_getter, _bool, null);
				m_PropertyDataConverter.SetBool(_bool);
				break;
			}
			
			case float: {
				float _float;
				g_Script.CallFunction(m_LayoutRoot, widget_getter, _float, null);
				m_PropertyDataConverter.SetFloat(_float);
				break;
			}			
			
			case string: {
				//string _string = "";
				//g_Script.CallFunction(m_LayoutRoot, widget_getter, _string, null);
				m_PropertyDataConverter.SetString(EditBoxWidget.Cast(m_LayoutRoot).GetText());
				break;
			}			
						
			
			default: {
				MVC.UnsupportedConversionError(m_PropertyDataConverter.Type(), m_WidgetDataType);
				return;
			}
		}
		

		m_PropertyDataConverter.SetToController(m_Controller, Binding_Name, Binding_Index);
		m_Controller.NotifyPropertyChanged(Binding_Name);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		
		if (button != 0) 
			return false;
		
		EditorLog.Trace("ViewBinding::OnClick " + w.Type());
		
		switch (w.Type()) {

			case ButtonWidget: {
				UpdateModel();
				return true;
			}			
		}
		
		return false;
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
				MVC.ErrorDialog(string.Format("Unknown Type Specified %1", widget_type));
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
				return "GetChildren";*/
			
		
			
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
				MVC.ErrorDialog(string.Format("Unknown Type Specified %1", widget_type));
			}
		}
		
		return string.Empty;
	}
	
	
	static typename GetWidgetDataType(typename widget_type)
	{
		switch (widget_type) {

			/* Observables
			case SpacerBaseWidget:
			case GridSpacerWidget:
			case WrapSpacerWidget:
			case ScrollWidget:
			case SpacerWidget:
			case Widget:
				return Widget;
			*/
						

			
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
				MVC.ErrorDialog(string.Format("Unknown Type Specified %1", widget_type));
			}
		}
		
		return Widget;
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


