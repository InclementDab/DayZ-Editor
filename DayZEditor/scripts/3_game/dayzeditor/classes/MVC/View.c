
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
				//SetModelVariable(SliderWidget.Cast(m_LayoutRoot).GetCurrent());
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
				
				TextWidgetData _TextWidgetData;
				EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _TextWidgetData);
				TextWidget.Cast(m_LayoutRoot).SetText(_TextWidgetData);
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
				_WrapSpacerWidgetData.Reload(WrapSpacerWidget.Cast(m_LayoutRoot));
				break;
			}
			
			case TextListboxWidget: {
				TextListboxWidgetData _TextListboxWidgetData;
				EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _TextListboxWidgetData);
				_TextListboxWidgetData.Reload(TextListboxWidget.Cast(m_LayoutRoot));
				break;
			}
			
			case XComboBoxWidget: {
				XComboBoxWidgetData _XComboBoxWidgetData;
				EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _XComboBoxWidgetData);
				_XComboBoxWidgetData.Reload(XComboBoxWidget.Cast(m_LayoutRoot));
				break;
			}
			
			default: {
				Error(string.Format("Unsupported Widget Type %1", m_LayoutRoot.Type().ToString()));
			}
			
		}
	}



	
	void OnCollectionChanged(CollectionChangedEventArgs args)
	{		
		EditorLog.Trace("EditorView::OnCollectionChanged: " + variable_name);
				
		Observable collection = args.param1;
		NotifyCollectionChangedAction action = args.param2;
		Param changed_params = args.param3;
				
		switch (m_LayoutRoot.Type()) {
			
			
			case WrapSpacerWidget: {
				
				WrapSpacerWidgetData wrap_data = WrapSpacerWidgetData.Cast(collection);
				if (wrap_data == null) {
					Error("WrapSpacerWidget: Wrong Type!");
					return;
				}
				
				switch (action) {
					case NotifyCollectionChangedAction.Add: {
						wrap_data.Add(m_LayoutRoot, changed_params);
						break;
					}
					
					case NotifyCollectionChangedAction.Remove: {
						wrap_data.Remove(m_LayoutRoot, changed_params);
						break;
					}
					
					case NotifyCollectionChangedAction.Clear: {
						wrap_data.Clear(m_LayoutRoot);
						break;
					}
					
					default: {
						Error("OnCollectionChanged: Not Implemented Exception!");
					}
				}
				
				break;
			}
			

			case TextListboxWidget: {
				
				TextListboxWidgetData listbox_data = TextListboxWidgetData.Cast(collection);				
				if (listbox_data == null) {
					Error("TextListboxWidget: Wrong Type!");
					return;
				}
				
				switch (action) {
					case NotifyCollectionChangedAction.Add: {
						listbox_data.Add(m_LayoutRoot, changed_params);
						break;
					}
					
					case NotifyCollectionChangedAction.Remove: {
						listbox_data.Remove(m_LayoutRoot);
						break;
					}
					
					case NotifyCollectionChangedAction.Clear: {
						listbox_data.Clear(m_LayoutRoot);
						break;
					}
					
					default: {
						Error("OnCollectionChanged: Not Implemented Exception!");
					}
				}
		
				break;
			}
			
			
			case XComboBoxWidget: {
								
				XComboBoxWidgetData combo_box_data = XComboBoxWidgetData.Cast(collection);				
				if (combo_box_data == null) {
					Error("XComboBoxWidgetData: Wrong Type!");
					return;
				}
				
				switch (action) {
					case NotifyCollectionChangedAction.Add: {
						combo_box_data.Add(m_LayoutRoot, changed_params);
						break;
					}
					
					case NotifyCollectionChangedAction.Remove: {
						combo_box_data.Remove(m_LayoutRoot);
						break;
					}
					
					case NotifyCollectionChangedAction.Clear: {
						combo_box_data.Clear(m_LayoutRoot);
						break;
					}
					
					default: {
						Error("OnCollectionChanged: Not Implemented Exception!");
					}
				}
				
				
				break;
			}
			
			
			default: {
				Error("OnCollectionChanged: Invalid Widget Type");
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