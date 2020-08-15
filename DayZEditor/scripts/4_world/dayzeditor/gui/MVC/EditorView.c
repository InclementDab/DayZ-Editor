
// TextListBoxWidget
typedef ObservableDictionary<string, Class> TextListboxWidgetData;

// WrapSpacerWidget
typedef ObservableCollection<Widget> WrapSpacerWidgetData;

// XComboBoxWidget
typedef ObservableCollection<string> XComboBoxWidgetData;

class EditorView extends ScriptedWidgetEventHandler
{
	// Required
	// Name of Widget that has ViewModel ScriptClass
	reference string view_model_widget;
	
	// Optional
	// if blank, will use name of Widget
	reference string variable_name; 
	reference int variable_index;
	// if blank, will use this widget
	reference string control_name; 
	
	private Widget m_LayoutRoot;
	private Widget m_ViewModelWidget;
	
	private ViewModelBase m_Model;
	private typename m_WidgetType;

	void EditorView()
	{
		EditorLog.Trace("EditorView");
		ViewModelBase.NotifyOnPropertyChanged(OnPropertyChanged);
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorView::OnWidgetScriptInit");
		if (view_model_widget == string.Empty) return;
		
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
				
		// Set the control widget to relevant Widget
		if (control_name != string.Empty) {
			m_LayoutRoot = m_LayoutRoot.FindAnyWidget(control_name);
		}
		
		// If var_name is blank, just use the name of root
		if (variable_name == string.Empty)
			variable_name = m_LayoutRoot.GetName();
		
		m_WidgetType = m_LayoutRoot.Type();
		m_ViewModelWidget = GetWidgetRoot(m_LayoutRoot).FindAnyWidget(view_model_widget);
		
		if (!m_ViewModelWidget) {
			Error(string.Format("ViewModel Widget not found! %1", view_model_widget));
			return;
		}
		
		m_ViewModelWidget.GetScript(m_Model);
		
		if (!m_Model) {
			Error(string.Format("%1 Could not find ViewModel: %2", m_LayoutRoot.GetName(), view_model_widget));
			return;
		}

		

		m_Model.InsertView(variable_name, this);
		
		typename var_type = m_Model.GetVariableType(variable_name);
		
		if (var_type.IsInherited(IObservable)) {
			EditorLog.Trace("Inherited from Observable: " + var_type.ToString());
			IObservable.NotifyOnCollectionChanged(OnCollectionChanged);
		}
	}
	
	void OnPropertyChanged(string property_name)
	{
		if (property_name != variable_name) return;
		EditorLog.Trace("EditorView::OnPropertyChanged: " + property_name);
		UpdateView();
	}
	
	
	// relies on observable_collection being true. totally bypasses regular events 10x faster
	void OnCollectionChanged(Class collection, NotifyCollectionChangedAction action, Param changed_params, string property_name)
	{
		if (property_name != variable_name) return;
		EditorLog.Trace("EditorView::OnCollectionChanged: " + property_name);
		
		switch (m_WidgetType) {
			
			// Param1: Widget
			case WrapSpacerWidget: {
				
				switch (action) {
					case NotifyCollectionChangedAction.Add: {
						m_LayoutRoot.AddChild(Param1<Widget>.Cast(changed_params).param1);
						break;
					}
					
					case NotifyCollectionChangedAction.Remove: {
						m_LayoutRoot.RemoveChild(Param1<Widget>.Cast(changed_params).param1);
						break;
					}
					
					default: {
						Error("OnCollectionChanged: Not Implemented Exception!");
					}
				}
				
				break;
			}
			
			// Param1: string
			// Param2: Class
			case TextListboxWidget: {
				
				TextListboxWidgetData listbox_data = TextListboxWidgetData.Cast(collection);
				TextListboxWidget listbox_widget = TextListboxWidget.Cast(m_LayoutRoot);
				Param2<string, Class> changed_data = Param2<string, Class>.Cast(changed_params);
				
				switch (action) {
					case NotifyCollectionChangedAction.Add: {
						listbox_widget.AddItem(changed_data.param1, changed_data.param2, 0);
						break;
					}
					
					case NotifyCollectionChangedAction.Remove: {
						listbox_widget.ClearItems();
						
						for (int i = 0; i < listbox_data.Count(); i++) {
							string key = listbox_data.GetKey(i);
							listbox_widget.AddItem(key, listbox_data.Get(key), 0);
						}
						
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
	

	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("EditorView::OnChange");
		if (m_Model) {
			UpdateModel();
			m_Model.NotifyPropertyChanged(w.GetName());
		}
		return true;
		//return super.OnChange(w, x, y, finished);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("OnClick");
		if (m_Model) {
			return m_Model.OnClick(w, x, y, button);
		}
	
		return super.OnClick(w, x, y, button);
	}
	
	
	// keeping this here if OnClick and OnChange dont work enough
	/*
	override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
	{
		return true;
		//return super.OnEvent(eventType, target, parameter0, parameter1);
	}*/

	
	
	// UI -> Model
	void UpdateModel()
	{
		EditorLog.Trace("EditorView::UpdateModel");
		if (!m_Model) return;
	
		string text;
		switch (m_WidgetType) {
			
			case TextWidget: {
				EditorLog.Warning("ViewModel cannot be updated for TextWidgets");
				break;
			}
			
			case MultilineEditBoxWidget: {
				MultilineEditBoxWidget.Cast(m_LayoutRoot).GetText(text);
				SetModelVariable(text);
				break;
			}
			
			case EditBoxWidget: {
				SetModelVariable(EditBoxWidget.Cast(m_LayoutRoot).GetText());
				break;
			}
			
			case ButtonWidget: {
				SetModelVariable(ButtonWidget.Cast(m_LayoutRoot).GetState());
				break;
			}
			
			case CheckBoxWidget: {
				SetModelVariable(CheckBoxWidget.Cast(m_LayoutRoot).IsChecked());
				break;
			}
			
			case TextListboxWidget: {
				TextListboxWidgetData data;
				TextListboxWidget list_box = TextListboxWidget.Cast(m_LayoutRoot);
				for (int i = 0; i < list_box.GetNumItems(); i++) {
					string title; Class list_data;
					list_box.GetItemText(i, 0, title);
					list_box.GetItemData(i, 0, list_data);
					data.Insert(title, list_data);
				}
				
				break;
			}
			
			case WrapSpacerWidget: {
				WrapSpacerWidgetData wrap_spacer_data(variable_name);
				WrapSpacerWidget wrap_spacer = WrapSpacerWidget.Cast(m_LayoutRoot);
				
				break;
			}
			
			case XComboBoxWidget: {
				//XComboBoxWidgetData combo_box_data = new XComboBoxWidgetData();
				XComboBoxWidget combo_box = XComboBoxWidget.Cast(m_LayoutRoot);
				
				for (int c = 0; c < combo_box.GetNumItems(); c++) {
					
				}
				
				break;
			}
			
			
			
			default: {
				Error(string.Format("Unsupported Widget Type %1", m_WidgetType.ToString()));
				break;
			}	
		}
	}
	
	// Model -> UI
	void UpdateView()
	{
		//EditorLog.Trace("EditorView::UpdateView");
		if (!m_Model) return;
		

		switch (m_WidgetType) {

			case TextWidget:
			case EditBoxWidget:
			case MultilineEditBoxWidget: {
				TextWidget.Cast(m_LayoutRoot).SetText(GetModelVariableString());
				break;
			}
		
			
			case ButtonWidget: {
				ButtonWidget.Cast(m_LayoutRoot).SetState(GetModelVariableBool());
				break;
			}
			
			case CheckBoxWidget: {
				CheckBoxWidget.Cast(m_LayoutRoot).SetChecked(GetModelVariableBool());
				break;
			} 
			
			
			
			case XComboBoxWidget: {
				XComboBoxWidgetData combo_box_data = new XComboBoxWidgetData(variable_name);
				XComboBoxWidget combo_box = XComboBoxWidget.Cast(m_LayoutRoot);
				
				if (EnScript.GetClassVar(m_Model, variable_name, 0, combo_box_data)) {
					Error(string.Format("Wrong Data Type in %1", m_LayoutRoot.GetName()));
					break;
				}
				
				for (int cbox = 0; cbox < combo_box_data.Count(); cbox++) {
					combo_box.AddItem(combo_box_data.Get(cbox));					
				}
				
				
				
				break;
			}
	
			
			default: {
				Error(string.Format("Unsupported Widget Type %1", m_WidgetType.ToString()));
				break;
			}	
		}
	}
	
	
	string GetModelVariableString()
	{
		string text;
		typename variable_type = m_Model.GetVariableType(variable_name);
		switch (variable_type) {
			
			case string: {
				EnScript.GetClassVar(m_Model, variable_name, variable_index, text);
				return text;
			}
			
			case float: {
				float text_float;
				EnScript.GetClassVar(m_Model, variable_name, variable_index, text_float);
				text = "" + text_float;
				return text;
			}
			
			case int: {
				int text_int;
				EnScript.GetClassVar(m_Model, variable_name, variable_index, text_int);
				text = "" + text_int;
				return text;
			}
			
			default: {
				Error(string.Format("Unsupported Type: %1", variable_type.ToString()));
				break;
			}
		}
		
		return string.Empty;
	}
	
	bool GetModelVariableBool()
	{
		bool state;
		EnScript.GetClassVar(m_Model, variable_name, variable_index, state);
		return state;
	}
	
	
	
	void SetModelVariable(TextListboxWidgetData data)
	{
		EnScript.SetClassVar(m_Model, variable_name, variable_index, data);
	}
	
	void SetModelVariable(string text)
	{
		typename variable_type = m_Model.GetVariableType(variable_name);
		switch (variable_type) {
					
			case string: {
				EnScript.SetClassVar(m_Model, variable_name, variable_index, text);
				break;
			}
			
			case float: {
				EnScript.SetClassVar(m_Model, variable_name, variable_index, text.ToFloat());
				break;
			}
			
			case int: {
				EnScript.SetClassVar(m_Model, variable_name, variable_index, text.ToInt());
				break;
			}
			
			default: {
				Error(string.Format("Unsupported Type: %1", variable_type.ToString()));
				break;
			}
		}
	}
	
	void SetModelVariable(bool state)
	{
		EnScript.SetClassVar(m_Model, variable_name, variable_index, state);
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