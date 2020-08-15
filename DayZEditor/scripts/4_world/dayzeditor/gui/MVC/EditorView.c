enum EditorViewType
{
	TEXT,
	CHECKBOX
};



typedef ref map<string, typename> VariableHashMap;

class EditorView extends ScriptedWidgetEventHandler
{
	reference string variable_name;
	//reference string variable_type; // m_ControlTypes
	reference int variable_index;
	reference string control_name;
	
	private Widget m_LayoutRoot;
	private Widget m_ControlWidget;
	
	private EditorViewType m_Type;
	private Managed m_Model;
	private ref VariableHashMap m_ModelHashMap;
	
	private const ref array<typename> supported_types = { int, float, string };
	
	
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorView::OnWidgetScriptInit");
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);

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
				Error("Unsupported type");
		}
		
		/*
		if (supported_types.Find(variable_type) == -1) {
			EditorLog.Error("Unsupported type");
		}*/
	}
	
	void SetModel(Managed model)
	{
		m_Model = model;
		
		m_ModelHashMap = new VariableHashMap();
		typename vtype = m_Model.Type();
		int vcnt = vtype.GetVariableCount();
		for (int i = 0; i < vcnt; i++) {
			m_ModelHashMap.Insert(vtype.GetVariableName(i), vtype.GetVariableType(i));
		}
		
		UpdateView();
	}
	
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (m_Model)
			UpdateView();
		
		return true;
	}

	void DebugPrint()
	{
		EditorLog.Debug("variable_name:" + variable_name);
		//EditorLog.Debug("variable_type:" + variable_type);
		EditorLog.Debug("variable_index:" + variable_index);
		EditorLog.Debug("control_name:" + control_name);
	}

	void UpdateView()
	{
		EditorLog.Trace("EditorView::UpdateView");
		switch (m_Type)
		{
			case EditorViewType.TEXT:
			{
				string text;
				TextWidget tw = TextWidget.Cast(m_ControlWidget);
				if (tw) {
					EditorLog.Error("ViewModel cannot be updated for TextWidgets");
				}
				
				MultilineEditBoxWidget mebw = MultilineEditBoxWidget.Cast(m_ControlWidget);
				if (mebw) {
					mebw.GetText(text);
				}
			
				EditBoxWidget ebw = EditBoxWidget.Cast(m_ControlWidget);
				if (ebw) {
					text = ebw.GetText();
				}
								
				
				//typename t = variable_type.ToType();
				//Class c = t.Spawn();
				/*
				switch (variable_type.ToType()) {
					
					case string: {
						c = text.ToString();
						break;
					}
					
					
				}*/
				
				typename type = m_ModelHashMap.Get(variable_name);
				
				switch (type) {
					
					case string: {
						EnScript.SetClassVar(m_Model, variable_name, variable_index, text);
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
		switch (m_Type)
		{
			case EditorViewType.TEXT: {
				string text;
			
				typename type = m_ModelHashMap.Get(variable_name);
				
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
	
	static EditorView GetUIProperty(Widget root, Managed model, string name)
	{
		EditorView view;
		root.FindAnyWidget(name).GetScript(view);
		view.SetModel(model);		
		return view;
	}
	
	static ref array<EditorView> GetUIProperties(Widget root, Managed model)
	{
		ref array<EditorView> views = new array<EditorView>();
		_GetUIProperties(root, model, views);
		return views;
	}
	
	private static void _GetUIProperties(Widget root, Managed model, inout array<EditorView> views)
	{
		EditorView view;
		root.GetScript(view);
		if (view != null) {
			view.SetModel(model);
			views.Insert(view);
		}
		
		Widget child = root.GetChildren();
		while (child != null) {
			_GetUIProperties(child, model, views);
			child = child.GetSibling();
		}
	}
};