


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
		
		m_WidgetController = GetWidgetController(m_LayoutRoot);
		if (!m_WidgetController) {
			MVC.UnsupportedTypeError(m_LayoutRoot.Type());
			return;
		}
		
		// Check for two way binding support
		if (Two_Way_Binding && !m_WidgetController.CanTwoWayBind()) {
			MVC.ErrorDialog(string.Format("Two Way Binding for %1 is not supported!", m_LayoutRoot.Type()));
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
		if (!Two_Way_Binding || !m_WidgetController.CanTwoWayBind()) 
			return;
		
		EditorLog.Trace("ViewBinding::UpdateModel");
		EditorLog.Debug(string.Format("[%1] Updating Model...", m_LayoutRoot.Type()));
		
		m_WidgetController.GetData(m_PropertyDataConverter);

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

		
		//m_PropertyDataConverter.GetFromController(m_Controller, Binding_Name, Binding_Index);
		EditorLog.Debug(string.Format("[%1] Updating Collection View...", m_LayoutRoot.Type()));

		
		switch (m_PropertyDataConverter.GetType()) {
			
			case bool: {
				m_PropertyDataConverter.SetBool(Param1<bool>.Cast(args.param4).param1);
				break;
			}
			
			case int:
			case float: {
				m_PropertyDataConverter.SetFloat(Param1<float>.Cast(args.param4).param1);
				break;
			}
			
			case string: {
				m_PropertyDataConverter.SetString(Param1<string>.Cast(args.param4).param1);
				break;
			}
			
			case Widget: {
				m_PropertyDataConverter.SetWidget(Param1<Widget>.Cast(args.param4).param1);
				break;
			}
			
			default: {
				MVC.UnsupportedTypeError(m_PropertyDataConverter.GetType());
			}
			
		}
		
		
		
		
		switch (args.param2) {
			
			case NotifyCollectionChangedAction.Add: {
				m_WidgetController.InsertData(args.param3, m_PropertyDataConverter);
				break;
			}
			
			case NotifyCollectionChangedAction.Remove: {
				m_WidgetController.RemoveData(args.param3);
				break;
			}
			
			case NotifyCollectionChangedAction.Set: {
				m_WidgetController.ReplaceData(args.param3, m_PropertyDataConverter);
				break;
			}
			
			case NotifyCollectionChangedAction.Move: {
				MVC.ErrorDialog(string.Format("Unsupported CollectionChangedAction: %1", args.param2));
				break;	
			}
			
			case NotifyCollectionChangedAction.Clear: {
				m_WidgetController.ClearData();
				break;
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
	
}


