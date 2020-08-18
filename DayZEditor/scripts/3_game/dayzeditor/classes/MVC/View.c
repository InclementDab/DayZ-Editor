


class EditorView: ScriptedWidgetEventHandler
{
	

	// Required
	// Name of Widget that has ViewModel ScriptClass
	private reference string view_model_widget;
	string GetViewModelWidgetName() { return view_model_widget; }
	
	//private reference bool Edit_Options;
		
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
		
	/*	
#ifdef COMPONENT_SYSTEM
		
		if (Edit_Options) {
			Edit_Options = false;
			
			if (variable_name == string.Empty) {
				variable_name = w.GetName();
			}
			
			EditorViewData options(view_model_widget, variable_name, variable_index, control_name);
			
			int result = m_MVCPlugin.ShowDialog(options);
			if (result > 0) {
				Print(options.ViewModelWidget);
				view_model_widget = options.ViewModelWidget;
				variable_name = options.VariableName;
				variable_index = options.VariableIndex;
				control_name = options.ControlName;
				
			} else if (result == -2) {
				// This only gets called if m_MVCPlugin is not set
				Error("Failed to load Workbench Module");
			}
		}
		
#endif
		*/
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
			Workbench.Dialog("Error", string.Format("ViewModel Widget not found! %1", view_model_widget));
			return;
		}
		
		m_ViewModelWidget.GetScript(m_ViewModel);
		
		if (!m_ViewModel) {
			Workbench.Dialog("Error", string.Format("%1 Could not find ViewModel: %2", m_LayoutRoot.GetName(), view_model_widget));
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
				MultilineEditBoxWidgetData _MultilineEditBoxWidgetData;
				MultilineEditBoxWidget _MultilineEditBoxWidget = MultilineEditBoxWidget.Cast(m_LayoutRoot);
				for (int i = 0; i < _MultilineEditBoxWidget.GetLinesCount(); i++) {
					string line;
					_MultilineEditBoxWidget.GetLine(i, line);
					EnScript.SetClassVar(m_ViewModel, variable_name, i, line);
				}

				break;
			}
			
			case EditBoxWidget: {
				Print(m_ViewModel.GetVariableType(variable_name));
				switch (m_ViewModel.GetVariableType(variable_name)) {
					
					case TextWidgetData:
					case string: {
						EditBoxWidgetData _EditBoxWidgetData = EditBoxWidget.Cast(m_LayoutRoot).GetText();
						EnScript.SetClassVar(m_ViewModel, variable_name, variable_index, _EditBoxWidgetData);
						break;
					}
					
					case EditBoxWidgetDataF: {
						EditBoxWidgetDataF _EditBoxWidgetDataF = EditBoxWidget.Cast(m_LayoutRoot).GetText();
						_EditBoxWidgetDataF = _EditBoxWidgetDataF.GetValidString();
						
						_EditBoxWidgetDataF = _EditBoxWidgetDataF.Evaluate();
						EnScript.SetClassVar(m_ViewModel, variable_name, variable_index, _EditBoxWidgetDataF);
						break;
					}
					
					
				}
				
				

				
				break;
			}
			
			case ButtonWidget: {
				ButtonWidgetData _ButtonWidgetData;
				_ButtonWidgetData = ButtonWidget.Cast(m_LayoutRoot).GetState();
				EnScript.SetClassVar(m_ViewModel, variable_name, variable_index, _ButtonWidgetData);
				break;
			}
			
			case CheckBoxWidget: {
				CheckBoxWidgetData _CheckBoxWidgetData;
				_CheckBoxWidgetData = CheckBoxWidget.Cast(m_LayoutRoot).IsChecked();
				EnScript.SetClassVar(m_ViewModel, variable_name, variable_index, _CheckBoxWidgetData);
				break;
			}
			
			case SliderWidget: {
				SliderWidgetData _SliderWidgetData;
				_SliderWidgetData = SliderWidget.Cast(m_LayoutRoot).GetCurrent();
				EnScript.SetClassVar(m_ViewModel, variable_name, variable_index, _SliderWidgetData);
				break;
			}
			
			case XComboBoxWidget: {

				break;
			}
			
			default: {
				Error(string.Format("Unsupported Widget Type %1", m_LayoutRoot.Type().ToString()));
			}
		}
		
		NotifyPropertyChanged(variable_name);
	}
	
	
	// Model -> UI
	void OnPropertyChanged() 
	{
		//EditorLog.Trace("EditorView::OnPropertyChanged: " + variable_name);
		
		switch (m_LayoutRoot.Type()) {
			
			case TextWidget: {
				
				switch (m_ViewModel.GetVariableType(variable_name)) {
					
					case TextWidgetData:
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
					
					case bool: {
						bool _TextWidgetDataB;
						EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _TextWidgetDataB);
						TextWidget.Cast(m_LayoutRoot).SetText(_TextWidgetDataB.ToString());
						break;
					}
				}
						
				break;
			}
			
			case EditBoxWidget: {
				
				switch (m_ViewModel.GetVariableType(variable_name)) {
					
					case EditBoxWidgetData:
					case EditBoxWidgetDataF:
					case string: {
						string _EditBoxWidgetDataS;
						EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _EditBoxWidgetDataS);
						EditBoxWidget.Cast(m_LayoutRoot).SetText(_EditBoxWidgetDataS);
						break;
					}
					
					case int:
					case float: {
						float _EditBoxWidgetDataF;
						EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _EditBoxWidgetDataF);
						EditBoxWidget.Cast(m_LayoutRoot).SetText(_EditBoxWidgetDataF.ToString());
						break;
					}
					
					case bool: {
						bool _EditBoxWidgetDataB;
						EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _EditBoxWidgetDataB);
						EditBoxWidget.Cast(m_LayoutRoot).SetText(_EditBoxWidgetDataB.ToString());
						break;
					}
					
					case vector: {
						vector _EditBoxWidgetDataV;
						EnScript.GetClassVar(m_ViewModel, variable_name, 0, _EditBoxWidgetDataV);
						EditBoxWidget.Cast(m_LayoutRoot).SetText(_EditBoxWidgetDataV[variable_index].ToString());
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