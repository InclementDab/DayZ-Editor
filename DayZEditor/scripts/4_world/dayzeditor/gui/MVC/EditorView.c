
// TextListboxWidget
class TextListboxWidgetData: ObservableDictionary<string, Class>
{
	void DictionaryAdd(TextListboxWidget w, Param params)
	{
		Param2<string, Class> changed_data = Param2<string, Class>.Cast(params);
		w.AddItem(changed_data.param1, changed_data.param2, 0);
	}
	
	void DictionaryRemove(TextListboxWidget w)
	{
		w.ClearItems();
		for (int i = 0; i < Count(); i++) {
			string key = GetKey(i);
			w.AddItem(key, Get(key), 0);
		}
	}
}

// WrapSpacerWidget
class WrapSpacerWidgetData: ObservableCollection<Widget>
{
	void DictionaryAdd(WrapSpacerWidget w, Param params)
	{
		w.AddChild(Param1<Widget>.Cast(params).param1);
	}
	
	void DictionaryRemove(WrapSpacerWidget w, Param params)
	{
		w.RemoveChild(Param1<Widget>.Cast(params).param1);
	}
}

// XComboBoxWidget
class XComboBoxWidgetData: ObservableCollection<string> 
{
	void DictionaryAdd(XComboBoxWidget w, Param params)
	{
		w.AddItem(Param1<string>.Cast(params).param1);
	}
	
	void DictionaryRemove(XComboBoxWidget w)
	{
		w.ClearAll();
		for (int i = 0; i < Count(); i++) 
			w.AddItem(Get(i));
	}
}

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
		m_Model.PropertyChanged.Insert(OnPropertyChanged);
		
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
				
				WrapSpacerWidgetData wrap_data = WrapSpacerWidgetData.Cast(collection);
				switch (action) {
					case NotifyCollectionChangedAction.Add: {
						wrap_data.DictionaryAdd(WrapSpacerWidget.Cast(m_LayoutRoot), changed_params);
						break;
					}
					
					case NotifyCollectionChangedAction.Remove: {
						wrap_data.DictionaryRemove(WrapSpacerWidget.Cast(m_LayoutRoot), changed_params);
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
				switch (action) {
					case NotifyCollectionChangedAction.Add: {
						listbox_data.DictionaryAdd(TextListboxWidget.Cast(m_LayoutRoot), changed_params);
						break;
					}
					
					case NotifyCollectionChangedAction.Remove: {
						listbox_data.DictionaryRemove(TextListboxWidget.Cast(m_LayoutRoot));
						break;
					}
					
					default: {
						Error("OnCollectionChanged: Not Implemented Exception!");
					}
				}
		
				break;
			}
			
			// Param1: string
			case XComboBoxWidget: {
								
				XComboBoxWidgetData combo_box_data = XComboBoxWidgetData.Cast(collection);				
				switch (action) {
					case NotifyCollectionChangedAction.Add: {
						combo_box_data.DictionaryAdd(XComboBoxWidget.Cast(m_LayoutRoot), changed_params);
						break;
					}
					
					case NotifyCollectionChangedAction.Remove: {
						combo_box_data.DictionaryRemove(XComboBoxWidget.Cast(m_LayoutRoot));
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
		}

		return super.OnChange(w, x, y, finished);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorView::OnClick");
		if (m_Model) {
			UpdateModel();
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
			
			case SliderWidget: {
				SetModelVariable(SliderWidget.Cast(m_LayoutRoot).GetCurrent());
				break;
			}
			
			case XComboBoxWidget: {
	
				int x = XComboBoxWidget.Cast(m_LayoutRoot).GetCurrentItem();
				GetEditor().SetBrush(GetEditorHudViewModel().GetBrush(GetEditorHudViewModel().m_TempBrushDir.Get(x)));
	
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
			
			case SliderWidget: {
				SliderWidget.Cast(m_LayoutRoot).SetCurrent(GetModelVariableFloat());
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
	
	float GetModelVariableFloat()
	{
		float value;
		EnScript.GetClassVar(m_Model, variable_name, variable_index, value);
		return value;
	}
	
	
	
	void SetModelVariable(TextListboxWidgetData data)
	{
		EnScript.SetClassVar(m_Model, variable_name, variable_index, data);
		m_Model.NotifyPropertyChanged(variable_name);
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
		
		m_Model.NotifyPropertyChanged(variable_name);
	}
	
	void SetModelVariable(bool state)
	{
		EnScript.SetClassVar(m_Model, variable_name, variable_index, state);
		m_Model.NotifyPropertyChanged(variable_name);
	}
	
	void SetModelVariable(float value)
	{
		EnScript.SetClassVar(m_Model, variable_name, variable_index, value);
		m_Model.NotifyPropertyChanged(variable_name);
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