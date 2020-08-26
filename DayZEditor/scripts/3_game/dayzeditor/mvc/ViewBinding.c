


class ViewBinding: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	protected reference string Binding_Name;
	protected reference int Binding_Index;
	protected reference bool Two_Way_Binding;
	
	Widget GetRoot() { return m_LayoutRoot; }
	string GetBindingName() { return Binding_Name; }
	int GetBindingIndex() { return Binding_Index; }
	
	private ref WidgetController m_WidgetController;
	private ref TypeConverter m_PropertyDataConverter;
	private typename m_PropertyDataType;
	
	private Controller m_Controller;
	
	void ViewBinding() { /*EditorLog.Trace("ViewBinding");*/ }
	void ~ViewBinding() { /*EditorLog.Trace("~ViewBinding");*/ }
	
	void SetController(Controller controller) 
	{ 
		EditorLog.Trace("ViewBinding::SetController");
		m_Controller = controller;
	
		m_PropertyDataType = m_Controller.GetPropertyType(Binding_Name);
		
		if (m_PropertyDataType.IsInherited(Observable)) {
			m_PropertyDataConverter = MVC.GetTypeConversion(Observable.Cast(m_PropertyDataType.Spawn()).GetType());
		} else {
			m_PropertyDataConverter = MVC.GetTypeConversion(m_PropertyDataType);
		}
		
		
		
		// Updates the view on first load
		if (m_PropertyDataConverter) {
			OnPropertyChanged();
			
		} else {
			EditorLog.Warning(string.Format("[%1] Data Converter not found!", m_LayoutRoot.GetName()));
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
		
		m_WidgetController = GetWidgetController(m_LayoutRoot);
		if (!m_WidgetController) {
			MVC.UnsupportedTypeError(m_LayoutRoot.Type());
			return;
		}
		
		
		m_LayoutRoot.SetHandler(this);
	}
	
	void OnPropertyChanged()
	{
		EditorLog.Trace("ViewBinding::OnPropertyChanged " + Binding_Name);		
						
		if (!m_PropertyDataType) {
			MVC.ErrorDialog(string.Format("Binding not found: %1", Binding_Name));
			return;
		}
		
		if (!m_PropertyDataConverter) {
			MVC.ErrorDialog(string.Format("Could not find TypeConversion for Type %1\nUse TypeConverter.RegisterTypeConversion to register custom types", m_PropertyDataType));
			return;			
		}
				
		m_PropertyDataConverter.GetFromController(m_Controller, Binding_Name, Binding_Index);
		EditorLog.Debug(string.Format("[%1] Updating View...", m_LayoutRoot.Type()));
		

		m_WidgetController.SetData(m_PropertyDataConverter);	
	}
	
	void UpdateModel()
	{
		if (!Two_Way_Binding || !SupportsTwoWayBinding(m_LayoutRoot.Type())) 
			return;
		
		EditorLog.Trace("ViewBinding::UpdateModel");
		EditorLog.Debug(string.Format("[%1] Updating Model...", m_LayoutRoot.Type()));
		
		m_WidgetController.GetData(m_PropertyDataConverter);
		
			/*	
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
		}*/
		

		m_PropertyDataConverter.SetToController(m_Controller, Binding_Name, Binding_Index);
		m_Controller.NotifyPropertyChanged(Binding_Name);
	}
	
	
	void OnCollectionChanged(CollectionChangedEventArgs args)
	{
		EditorLog.Trace("ViewBinding::OnCollectionChanged " + Binding_Name);
		
		if (!m_PropertyDataType) {
			MVC.ErrorDialog(string.Format("Binding not found: %1", Binding_Name));
			return;
		}
		
		if (!m_PropertyDataConverter) {
			MVC.ErrorDialog(string.Format("Could not find TypeConversion for Type %1\nUse TypeConverter.RegisterTypeConversion to register custom types", m_PropertyDataType));
			return;			
		}

		
		m_PropertyDataConverter.GetFromController(m_Controller, Binding_Name, Binding_Index);
		EditorLog.Debug(string.Format("[%1] Updating Collection View...", m_LayoutRoot.Type()));


		
		switch (args.param2) {
			
			case NotifyCollectionChangedAction.Add:
			case NotifyCollectionChangedAction.Remove: {
				
				break;
			}
						
			default: {
				MVC.ErrorDialog(string.Format("Unsupported CollectionChangedAction: %1", args.param2));
			}
		}
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


