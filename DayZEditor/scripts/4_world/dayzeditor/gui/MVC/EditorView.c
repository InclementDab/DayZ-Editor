
// TextListBoxWidget
typedef ref map<string, Class> TextListboxWidgetData;

// WrapSpacerWidget
typedef ref array<ref Widget> WrapSpacerWidgetData;

// XComboBoxWidget
typedef ref set<string> XComboBoxWidgetData;

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
			Error("ViewModel Widget not found!");
			return;
		}
		
		m_ViewModelWidget.GetScript(m_Model);
		
		if (!m_Model) {
			Error(string.Format("%1 Could not find ViewModel: %2", m_LayoutRoot.GetName(), view_model_widget));
			return;
		}
		
		m_Model.InsertView(this);
		UpdateView();
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		Print("OnChange");
		if (m_Model) {
			UpdateModel();
			m_Model.OnPropertyChanged(w);
		}
		return true;
		//return super.OnChange(w, x, y, finished);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("OnClick");
		if (m_Model) {
			UpdateModel();
			m_Model.OnPropertyChanged(w);
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
				TextListboxWidgetData data = new TextListboxWidgetData();
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
				WrapSpacerWidgetData wrap_spacer_data = new WrapSpacerWidgetData();
				WrapSpacerWidget wrap_spacer = WrapSpacerWidget.Cast(m_LayoutRoot);
				
				break;
			}
			
			case XComboBoxWidget: {
				XComboBoxWidgetData combo_data = new XComboBoxWidgetData();
				XComboBoxWidget combo_box = XComboBoxWidget.Cast(m_LayoutRoot);
				
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
			
			case TextListboxWidget: {
				TextListboxWidget list_box = TextListboxWidget.Cast(m_LayoutRoot);
				
				TextListboxWidgetData list_data = new TextListboxWidgetData();
				EnScript.GetClassVar(m_Model, variable_name, 0, list_data);
				list_box.ClearItems();

				
				foreach (string text, Class data: list_data)
					list_box.AddItem(text, data, 0);
				
							
				break;
			}
			
			case WrapSpacerWidget: {
				WrapSpacerWidgetData wrap_spacer_data = new WrapSpacerWidgetData();
				WrapSpacerWidget wrap_spacer = WrapSpacerWidget.Cast(m_LayoutRoot);

				if (EnScript.GetClassVar(m_Model, variable_name, 0, wrap_spacer_data)) {
					Error(string.Format("Wrong Data Type in %1", m_LayoutRoot.GetName()));
					break;
				}
				
				
				ClearWidgetChildren(wrap_spacer);
				
				foreach (Widget w: wrap_spacer_data) {
					wrap_spacer.AddChild(w);
				}
			
				
				
				
				break;
			}
			
			case XComboBoxWidget: {
				XComboBoxWidgetData combo_box_data = new XComboBoxWidgetData();
				XComboBoxWidget combo_box = XComboBoxWidget.Cast(m_LayoutRoot);
				
				if (EnScript.GetClassVar(m_Model, variable_name, 0, combo_box_data)) {
					Error(string.Format("Wrong Data Type in %1", m_LayoutRoot.GetName()));
					break;
				}
				
				foreach (string item: combo_box_data) {
					combo_box.AddItem(item);
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