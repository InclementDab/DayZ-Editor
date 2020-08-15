

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
	private Widget m_ControlWidget;
	
	private ViewModelBase m_Model;
	private typename m_WidgetType;

	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorView::OnWidgetScriptInit");
		if (view_model_widget == string.Empty) return;
		
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
				
		// Set the control widget to relevant Widget
		m_ControlWidget = m_LayoutRoot.FindAnyWidget(control_name);
		if (m_ControlWidget == null)
			m_ControlWidget = m_LayoutRoot;
		
		// If var_name is blank, just use the name of root
		if (variable_name == string.Empty)
			variable_name = m_ControlWidget.GetName();
		
		m_WidgetType = m_ControlWidget.Type();
		m_ViewModelWidget = GetWidgetRoot(m_LayoutRoot).FindAnyWidget(view_model_widget);
		
		if (!m_ViewModelWidget) {
			Error("ViewModel Widget not found!");
			return;
		}
		
		m_ViewModelWidget.GetScript(m_Model);
		
		if (!m_Model) {
			Error(string.Format("%1 Could not find ViewModel: %2", m_LayoutRoot.GetName(), view_model_widget));
			return;
		}
		
		m_Model.InsertView(this);
		UpdateModel();
	}
	

	override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
	{
		if (m_Model) {
			
			UpdateModel();
			m_Model.OnPropertyChanged(variable_name);
		}
		
		return super.OnEvent(eventType, target, parameter0, parameter1);
	}

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
				MultilineEditBoxWidget.Cast(m_ControlWidget).GetText(text);
				SetModelVariable(text);
				break;
			}
			
			case EditBoxWidget: {
				SetModelVariable(EditBoxWidget.Cast(m_ControlWidget).GetText());
				break;
			}
			
			case ButtonWidget: {
				SetModelVariable(ButtonWidget.Cast(m_ControlWidget).GetState());
				break;
			}
			
			case CheckBoxWidget: {
				SetModelVariable(CheckBoxWidget.Cast(m_ControlWidget).IsChecked());
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
		EditorLog.Trace("EditorView::UpdateView");
		if (!m_Model) return;
		
		switch (m_WidgetType) {

			case TextWidget:
			case EditBoxWidget:
			case MultilineEditBoxWidget: {
				TextWidget.Cast(m_ControlWidget).SetText(GetModelVariableString());
				break;
			}
		
			
			case ButtonWidget: {
				ButtonWidget.Cast(m_ControlWidget).SetState(GetModelVariableBool());
				break;
			}
			
			case CheckBoxWidget: {
				CheckBoxWidget.Cast(m_ControlWidget).SetChecked(GetModelVariableBool());
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