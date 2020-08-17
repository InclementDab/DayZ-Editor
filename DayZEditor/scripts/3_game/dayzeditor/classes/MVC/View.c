
class EditorView: ScriptedWidgetEventHandler
{
	// Required
	// Name of Widget that has ViewModel ScriptClass
	private reference string view_model_widget;
	string GetViewModelWidgetName() { return view_model_widget; }
	
	// Optional
	// if blank, will use name of Widget
	private reference string variable_name;
	string GetVariableName() { return variable_name; } 
	
	// Index of array if using arrays
	private reference int variable_index;
	int GetVariableIndex() { return variable_index; }
	
	// if blank, will use this widget
	private reference string control_name; 
	string GetControlName() { return control_name; } 
	
	private Widget m_LayoutRoot;
	private Widget m_ViewModelWidget;
	
	private ViewModel m_ViewModel;

	void EditorView() { EditorLog.Trace("EditorView"); }
	void ~EditorView() { EditorLog.Trace("~EditorView"); }
	
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorView::OnWidgetScriptInit");
		if (view_model_widget == string.Empty) return;
		
		m_LayoutRoot = w;
		
				
		// Set the control widget to relevant Widget
		if (control_name != string.Empty) {
			m_LayoutRoot = m_LayoutRoot.FindAnyWidget(control_name);
		}
		
		// If var_name is blank, just use the name of root
		if (variable_name == string.Empty)
			variable_name = m_LayoutRoot.GetName();
		
		m_ViewModelWidget = GetWidgetRoot(m_LayoutRoot).FindAnyWidget(view_model_widget);
		
		if (!m_ViewModelWidget) {
			Error(string.Format("ViewModel Widget not found! %1", view_model_widget));
			return;
		}
		
		m_ViewModelWidget.GetScript(m_ViewModel);
		
		if (!m_ViewModel) {
			Error(string.Format("%1 Could not find ViewModel: %2", m_LayoutRoot.GetName(), view_model_widget));
			return;
		}
		
		m_ViewModel.InsertView(this);
		m_LayoutRoot.SetHandler(this);
		
				
		// This reloads the view incase data is added before view is created
		OnPropertyChanged();
	}


	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("EditorView::OnChange");
		UpdateModel();
		return super.OnChange(w, x, y, finished);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorView::OnClick");
		UpdateModel();
		return super.OnClick(w, x, y, button);
	}
	

	
	// UI -> Model
	void UpdateModel()
	{
		EditorLog.Trace("EditorView::UpdateModel");
	
		string text;
		switch (m_LayoutRoot.Type()) {
			
			case TextWidget: {
				EditorLog.Warning("ViewModel cannot be updated for TextWidgets");
				break;
			}
			
			case MultilineEditBoxWidget: {
				MultilineEditBoxWidget.Cast(m_LayoutRoot).GetText(text);
				//SetModelVariable(text);
				break;
			}
			
			case EditBoxWidget: {
				//SetModelVariable(EditBoxWidget.Cast(m_LayoutRoot).GetText());
				break;
			}
			
			case ButtonWidget: {
				//SetModelVariable(ButtonWidget.Cast(m_LayoutRoot).GetState());
				break;
			}
			
			case CheckBoxWidget: {
				//SetModelVariable(CheckBoxWidget.Cast(m_LayoutRoot).IsChecked());
				break;
			}
			
			case SliderWidget: {
				SliderWidgetData _SliderWidgetData;
				_SliderWidgetData = SliderWidget.Cast(m_LayoutRoot).GetCurrent();
				EnScript.SetClassVar(m_ViewModel, variable_name, variable_index, _SliderWidgetData);
				NotifyPropertyChanged(variable_name);
				
				//SetModelVariable();
				break;
			}
			
			case XComboBoxWidget: {

				break;
			}
			
			default: {
				Error(string.Format("Unsupported Widget Type %1", m_LayoutRoot.Type().ToString()));
			}	
		}
	}
	
	
	// Model -> UI
	void OnPropertyChanged() 
	{
		//EditorLog.Trace("EditorView::OnPropertyChanged: " + variable_name);
		
		switch (m_LayoutRoot.Type()) {
			
			case TextWidget: {
				
				switch (m_ViewModel.GetVariableBaseType(variable_name)) {
					
					
					case string: {
						string _TextWidgetDataS;
						EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _TextWidgetDataS);
						TextWidget.Cast(m_LayoutRoot).SetText(_TextWidgetDataS);
						break;
					}
					
					case int:
					case float: {
						float _TextWidgetDataF;
						EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _TextWidgetDataF);
						TextWidget.Cast(m_LayoutRoot).SetText(_TextWidgetDataF.ToString());
						break;
					}				
				}
						
				
				break;
			}
			
			case ButtonWidget: {
				
				ButtonWidgetData _ButtonWidgetData;
				EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _ButtonWidgetData);
				ButtonWidget.Cast(m_LayoutRoot).SetState(_ButtonWidgetData);
				break;
			}
			
			case CheckBoxWidget: {
				
				CheckBoxWidgetData _CheckBoxWidgetData;
				EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _CheckBoxWidgetData);
				CheckBoxWidget.Cast(m_LayoutRoot).SetChecked(_CheckBoxWidgetData);
				break;
			}
			
			case SliderWidget: {
				SliderWidgetData _SliderWidgetData;
				EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _SliderWidgetData);
				SliderWidget.Cast(m_LayoutRoot).SetCurrent(_SliderWidgetData);
				break;
			}
			
			case WrapSpacerWidget: {
				WrapSpacerWidgetData _WrapSpacerWidgetData;
				EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _WrapSpacerWidgetData);
				_WrapSpacerWidgetData.ReloadData(WrapSpacerWidget.Cast(m_LayoutRoot));
				break;
			}
			
			case TextListboxWidget: {
				TextListboxWidgetData _TextListboxWidgetData;
				EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _TextListboxWidgetData);
				_TextListboxWidgetData.ReloadData(TextListboxWidget.Cast(m_LayoutRoot));
				break;
			}
			
			case XComboBoxWidget: {
				XComboBoxWidgetData _XComboBoxWidgetData;
				EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _XComboBoxWidgetData);
				_XComboBoxWidgetData.ReloadData(XComboBoxWidget.Cast(m_LayoutRoot));
				break;
			}
			
			default: {
				Error(string.Format("Unsupported Widget Type %1", m_LayoutRoot.Type().ToString()));
			}
			
		}
	}



	
	void OnCollectionChanged(CollectionChangedEventArgs args)
	{		
		EditorLog.Trace(string.Format("EditorView::OnCollectionChanged: %1 Action: %2", variable_name, args.param2));
				
		Observable collection = args.param1;
		NotifyCollectionChangedAction action = args.param2;
		ref Param changed_params = args.param3;
		

		
		IWidgetData widget_data_interface(collection);
		
		switch (action) {
			case NotifyCollectionChangedAction.Add: {
				widget_data_interface.Add(m_LayoutRoot, changed_params);
				break;
			}
			
			case NotifyCollectionChangedAction.Remove: {
				widget_data_interface.Remove(m_LayoutRoot, changed_params);
				break;
			}
			
			case NotifyCollectionChangedAction.Clear: {
				widget_data_interface.Clear(m_LayoutRoot);
				break;
			}
			
			case NotifyCollectionChangedAction.Set: {
				widget_data_interface.Set(m_LayoutRoot, changed_params);
				break;
			}
			
			default: {
				Error("OnCollectionChanged: Not Implemented Exception!");
			}
		}	
	}
	
	
	
	
	void DebugPrint()
	{
		EditorLog.Debug("EditorView::DebugPrint: " + m_LayoutRoot.GetName());
		EditorLog.Debug("view_model_widget:" + view_model_widget);
		EditorLog.Debug("variable_name:" + variable_name);
		EditorLog.Debug("variable_index:" + variable_index);
		EditorLog.Debug("control_name:" + control_name);
	}
};