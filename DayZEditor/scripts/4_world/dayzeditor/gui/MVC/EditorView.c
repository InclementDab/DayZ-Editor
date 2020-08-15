enum EditorViewType
{
	TEXT,
	BOOL
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

	private typename m_WidgetType;

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
		
		m_WidgetType = m_ControlWidget.Type();
		
		/*
		switch (m_ControlWidget.GetTypeID())
		{
			case TextWidgetTypeID:
			case EditBoxWidgetTypeID:
			case MultilineEditBoxWidgetTypeID:
				m_Type = EditorViewType.TEXT;
				break;
			case ButtonWidgetTypeID:
			case CheckBoxWidgetTypeID:
				m_Type = EditorViewType.BOOL;
				break;
			default:
				Error(w.GetName() + " Unsupported type");
				return;
		}*/
		
		
		

	
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
				
				TextWidget tw;
				if (Class.CastTo(tw, m_ControlWidget)) {
					EditorLog.Warning("ViewModel cannot be updated for TextWidgets");
				} 
				
				MultilineEditBoxWidget ml;
				if (Class.CastTo(ml, m_ControlWidget)) {
					ml.GetText(text);
				}
							
				EditBoxWidget eb;
				if (Class.CastTo(eb, m_ControlWidget)) {
					text = eb.GetText();
				}
				
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
			
				return;
			}
			case EditorViewType.BOOL: {
				bool state;
				
				
				CheckBoxWidget cb;
				if (Class.CastTo(cb, m_ControlWidget)) {
					state = cb.IsChecked();
				}
				
				ButtonWidget bw;
				if (Class.CastTo(bw, m_ControlWidget)) {
					state = bw.GetState();
				}
				
				EnScript.SetClassVar(m_Model, variable_name, variable_index, state);
				
				break;
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
				typename variable_type = m_Model.GetVariableType(variable_name);
				switch (variable_type) {
					
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
					
					default: {
						Error(string.Format("Unsupported Type: %1", variable_type.ToString()));
						break;
					}
				}
				
				
				TextWidget tw;
				if (Class.CastTo(tw, m_ControlWidget)) {
					tw.SetText(text);
				} 
				
				MultilineEditBoxWidget ml;
				if (Class.CastTo(ml, m_ControlWidget)) {
					ml.SetText(text);
				}
							
				EditBoxWidget eb;
				if (Class.CastTo(eb, m_ControlWidget)) {
					eb.SetText(text);
				}
				
				break;
			}
			
			case EditorViewType.BOOL: {
				bool state;
				EnScript.GetClassVar(m_Model, variable_name, variable_index, state);
				
				CheckBoxWidget cb;
				if (Class.CastTo(cb, m_ControlWidget)) {
					cb.SetChecked(state);
				}
				
				ButtonWidget bw;
				if (Class.CastTo(bw, m_ControlWidget)) {
					bw.SetState(state);
				}
				
				break;
			}
			
			default: {
				Error(string.Format("Unsupported EditorViewType %1", m_Type));
				break;
			}
		}
	}
	

};