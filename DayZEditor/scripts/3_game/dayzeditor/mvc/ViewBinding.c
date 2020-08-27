


class ViewBinding: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	// Name of Binding. If blank, uses the Widget name (not advised)
	reference string Binding_Name;
	
	// Function called when type is Clicked, Selected, or Changed
	reference string Command_Execute;
	
	// Function that returns bool defining whether or not the Command_Execute can be called
	reference string Command_CanExecute;
	
	// Only valid if type is ObservableCollection
	reference string Selected_Item;
	
	// If true, Bindings go both ways. Otherwise the controller is the master
	reference bool Two_Way_Binding;
	
	
	Widget GetRoot() { 
		return m_LayoutRoot; 
	}
	
	string GetBindingName() { 
		return Binding_Name; 
	}
	
	typename GetPropertyType(string property_name) {
		return m_Controller.GetPropertyType(property_name);
	}
	
	protected typename m_PropertyType;
	
	protected Controller m_Controller;
	protected ref TypeConverter m_PropertyDataConverter;
	protected ref WidgetController m_WidgetController;
	
	protected ref TypeConverter m_SelectedDataConverter;
	
	void ViewBinding() { /*EditorLog.Trace("ViewBinding");*/ }
	void ~ViewBinding() { /*EditorLog.Trace("~ViewBinding");*/ }
	
	void SetController(Controller controller) 
	{ 
		EditorLog.Trace("ViewBinding::SetController: %1", controller.GetLayoutRoot().GetName());
		m_Controller = controller;
		m_PropertyType = GetPropertyType(Binding_Name);
		
		if (!m_PropertyType) {
			MVC.PropertyNotFoundError(Binding_Name);
		}

		if (Selected_Item != string.Empty)
			m_SelectedDataConverter = MVC.GetTypeConversion(GetPropertyType(Selected_Item));
				
		if (m_PropertyType.IsInherited(Observable)) {			
			m_PropertyDataConverter = MVC.GetTypeConversion(Observable.Cast(m_PropertyType.Spawn()).GetType());
		} else {
			m_PropertyDataConverter = MVC.GetTypeConversion(m_PropertyType);
		}
		
		// Updates the view on first load
		UpdateView();
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("[%1] ViewBinding::Init", w.GetName());
		m_LayoutRoot = w;
		
		if (Binding_Name == string.Empty) {
			Binding_Name = m_LayoutRoot.GetName();
		}
		
		m_WidgetController = GetWidgetController(m_LayoutRoot);
		if (!m_WidgetController) {
			MVC.UnsupportedTypeError(m_LayoutRoot.Type());
		}
		
		// Check for two way binding support
		if (Two_Way_Binding && !m_WidgetController.CanTwoWayBind()) {
			EditorLog.Error("Two Way Binding for %1 is not supported!", m_LayoutRoot.Type().ToString());
		}
		

		m_LayoutRoot.SetHandler(this);
	}
	
	void OnPropertyChanged()
	{
		EditorLog.Trace("ViewBinding::OnPropertyChanged: %1", Binding_Name);
		UpdateView();
	}

	void OnCollectionChanged(ref CollectionChangedEventArgs args)
	{
		EditorLog.Trace("ViewBinding::OnCollectionChanged: %1", Binding_Name);

		
		if (!m_PropertyDataConverter) {
			MVC.TypeConversionError(m_PropertyType);
			return;			
		}
		
		if (!m_WidgetController) {
			MVC.UnsupportedTypeError(m_LayoutRoot.Type());
			return;
		}

		EditorLog.Debug("Updating Collection View: %1", m_LayoutRoot.Type().ToString());
			
		// Anonymouse Data Setter
		m_PropertyDataConverter.SetParam(args.param4);
		switch (args.param2) {
						
			case NotifyCollectionChangedAction.Add: {
				m_WidgetController.InsertData(args.param3, m_PropertyDataConverter);
				break;
			}
			
			case NotifyCollectionChangedAction.Remove: {
				m_WidgetController.RemoveData(args.param3, m_PropertyDataConverter);
				break;
			}
			
			case NotifyCollectionChangedAction.Set: {
				m_WidgetController.ReplaceData(args.param3, m_PropertyDataConverter);
				break;
			}
			
			case NotifyCollectionChangedAction.Move: {
				// this ones a weird case /shrug
				m_WidgetController.MoveData(args.param3, Param1<int>.Cast(args.param4).param1);
				break;	
			}
			
			case NotifyCollectionChangedAction.Clear: {
				m_WidgetController.ClearData();
				break;
			}
		}
	}
	
	private void UpdateView()
	{
		EditorLog.Trace("ViewBinding::UpdateView");
		EditorLog.Debug("Updating View: %1", m_LayoutRoot.Type().ToString());

		if (!m_PropertyDataConverter) {
			MVC.TypeConversionError(m_PropertyType);
			return;
		}
		
		if (m_SelectedDataConverter) {
			m_SelectedDataConverter.GetFromController(m_Controller, Selected_Item, 0);
			m_WidgetController.SetSelection(m_SelectedDataConverter);
		}

		m_PropertyDataConverter.GetFromController(m_Controller, Binding_Name, 0);
		m_WidgetController.SetData(m_PropertyDataConverter);
	}
	
	private void UpdateModel()
	{
		EditorLog.Trace("ViewBinding::UpdateModel");
		EditorLog.Debug("Updating Model: %1", m_LayoutRoot.Type().ToString());
		
		if (!m_PropertyDataConverter) {
			MVC.TypeConversionError(m_PropertyType);
			return;
		}
		
		if (m_SelectedDataConverter) {
			m_WidgetController.GetSelection(m_SelectedDataConverter);
			m_SelectedDataConverter.SetToController(m_Controller, Selected_Item, 0);
			m_Controller.NotifyPropertyChanged(Selected_Item);
		}
		
		if (!Two_Way_Binding || !m_WidgetController.CanTwoWayBind()) {
			m_Controller.NotifyPropertyChanged(Binding_Name);
			return;
		}

				
		m_WidgetController.GetData(m_PropertyDataConverter);
		m_PropertyDataConverter.SetToController(m_Controller, Binding_Name, 0);
		m_Controller.NotifyPropertyChanged(Binding_Name);
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{

		EditorLog.Trace("ViewBinding::OnClick " + w.Type());
		
		switch (w.Type()) {
			case ButtonWidget: {
				UpdateModel();
				InvokeCommand(new ButtonCommandArgs(ButtonWidget.Cast(w), button, ButtonWidget.Cast(w).GetState()));
				break;
			}
		}
		
		return false;
	}
	
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("ViewBinding::OnChange " + w.GetName());
		
		
		switch (w.Type()) {
			
			case CheckBoxWidget: {
				InvokeCommand(new CheckBoxCommandArgs(CheckBoxWidget.Cast(w), CheckBoxWidget.Cast(w).IsChecked()));
				break;
			}
			
			case XComboBoxWidget: {
				InvokeCommand(new XComboBoxCommandArgs(XComboBoxWidget.Cast(w), XComboBoxWidget.Cast(w).GetCurrentItem()));
				break;
			}
			
		}

		UpdateModel();
		
		return false;
		//return super.OnChange(w, x, y, finished);
	}
		
	
	
	void InvokeCommand(Param params)
	{
		if (Command_Execute == string.Empty) {
			return;
		}
				
		EditorLog.Trace("ViewBinding::InvokeCommand");
		g_Script.CallFunction(m_Controller, Command_Execute, null, params);
	}
	
	void UpdateCommand()
	{
		if (Command_CanExecute == string.Empty) {
			return;
		}
		
		EditorLog.Trace("ViewBinding::UpdateCommand");
		
		bool result;
		if (g_Script.CallFunction(m_Controller, Command_CanExecute, result, 0) != 0) {
			return;
		}
		
		if (result) {
			m_LayoutRoot.ClearFlags(WidgetFlags.IGNOREPOINTER);
		} else {
			m_LayoutRoot.SetFlags(WidgetFlags.IGNOREPOINTER);
			if (GetFocus() == m_LayoutRoot) {
				SetFocus(null);
			}
		}
	}
}


