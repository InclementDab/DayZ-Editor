enum EditorViewType
{
	TEXT,
	CHECKBOX
};



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
	
	private EditorViewType m_Type;
	private ViewModelBase m_Model;
	
	
	private const ref array<typename> supported_types = { int, float, string };

	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorView::OnWidgetScriptInit");
		if (view_model_widget == string.Empty) return;
		
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
		
		
		if (variable_name == string.Empty)
			variable_name = m_LayoutRoot.GetName();
		
		
		m_ControlWidget = m_LayoutRoot.FindAnyWidget(control_name);
		if (m_ControlWidget == null)
			m_ControlWidget = m_LayoutRoot;
		
		switch (m_ControlWidget.GetTypeID())
		{
			case TextWidgetTypeID:
			case EditBoxWidgetTypeID:
			case MultilineEditBoxWidgetTypeID:
				m_Type = EditorViewType.TEXT;
				break;
			case CheckBoxWidgetTypeID:
				m_Type = EditorViewType.CHECKBOX;
				break;
			default:
				Error(w.GetName() + " Unsupported type");
				return;
		}
		

	
		m_ViewModelWidget = GetWidgetRoot(m_LayoutRoot).FindAnyWidget(view_model_widget);
		
		if (!m_ViewModelWidget) {
			Error("ViewModel not found!");
			return;
		}
		
		m_ViewModelWidget.GetScript(m_Model);
		
		if (!m_Model) {
			Error("Error on ViewModel GetScript!");
			return;
		}
		
		m_Model.InsertView(this);
		UpdateView();
		
		
		/*
		if (supported_types.Find(variable_type) == -1) {
			EditorLog.Error("Unsupported type");
		}*/
	}
	
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("EditorView::OnChange");
		if (m_Model)
			UpdateView();
		
		return true;
	}

	void DebugPrint()
	{
		EditorLog.Debug("EditorView::DebugPrint: " + m_LayoutRoot.GetName());
		EditorLog.Debug("view_model_widget:" + view_model_widget);
		EditorLog.Debug("variable_name:" + variable_name);
		EditorLog.Debug("variable_index:" + variable_index);
		EditorLog.Debug("control_name:" + control_name);
	}

	void UpdateView()
	{
		EditorLog.Trace("EditorView::UpdateView");
		if (!m_Model) return;
		switch (m_Type)
		{
			case EditorViewType.TEXT:
			{
				string text;
				TextWidget tw = TextWidget.Cast(m_ControlWidget);
				if (tw) {
					EditorLog.Warning("ViewModel cannot be updated for TextWidgets");
				}
				
				MultilineEditBoxWidget mebw = MultilineEditBoxWidget.Cast(m_ControlWidget);
				if (mebw) {
					mebw.GetText(text);
				}
			
				EditBoxWidget ebw = EditBoxWidget.Cast(m_ControlWidget);
				if (ebw) {
					text = ebw.GetText();
				}
				
				
				
				switch (m_Model.GetVariableType(variable_name)) {
					
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
					
				}
				
				
				
			
				return;
			}
			case EditorViewType.CHECKBOX: {
				EnScript.SetClassVar(m_Model, variable_name, variable_index, CheckBoxWidget.Cast(m_ControlWidget).IsChecked());
				return;
			}
		}
	}
	
	void UpdateModel()
	{
		EditorLog.Trace("EditorView.UpdateModel");
		if (!m_Model) return;
		
		switch (m_Type)
		{
			case EditorViewType.TEXT: {
				string text;
			
				typename type = m_Model.GetVariableType(variable_name);
				
				switch (type) {
					
					case string: {
						EnScript.GetClassVar(m_Model, variable_name, variable_index, text);
						break;
					}
					
					case float: {
						float text_float;
						EnScript.GetClassVar(m_Model, variable_name, variable_index, text_float);
						text = "" + text_float;
						break;
					}
					
					case int: {
						int text_int;
						EnScript.GetClassVar(m_Model, variable_name, variable_index, text_int);
						text = "" + text_int;
						break;
					}
				}
				
				TextWidget tw = TextWidget.Cast(m_ControlWidget);
				if (tw) {
					tw.SetText(text);
				}
				
				MultilineEditBoxWidget mebw = MultilineEditBoxWidget.Cast(m_ControlWidget);
				if (mebw) {
					mebw.SetText(text);
					return;
				}
			
				EditBoxWidget ebw = EditBoxWidget.Cast(m_ControlWidget);
				if (ebw) {
					ebw.SetText(text);
					return;
				}
				
				
			
				return;
				
			}
			
			case EditorViewType.CHECKBOX: {
				bool checked;
				EnScript.GetClassVar(m_Model, variable_name, variable_index, checked);
			
				CheckBoxWidget.Cast(m_ControlWidget).SetChecked(checked);
				return;
			}
		}
	}
	

};