enum EditorViewType
{
	TEXT,
	CHECKBOX
};

class EditorView extends ScriptedWidgetEventHandler
{
	reference string variable_name;
	reference string variable_type; // "float" "int" "string"
	reference int variable_index;
	
	reference string control_name;
	
	private Widget m_LayoutRoot;
	private Widget m_ControlWidget;
	
	private EditorViewType m_Type;
	
	private Managed m_Model;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
		
		m_ControlWidget = m_LayoutRoot.FindAnyWidget( control_name );
		if (m_ControlWidget == null)
			m_ControlWidget = m_LayoutRoot;
		
		switch (m_ControlWidget.GetTypeID())
		{
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
		
		
		if (variable_type == "float")
		{
			Print("Creating as float");
		} else if (variable_type == "int")
		{
			Print("Creating as int");
		} else if (variable_type == "string")
		{
			Print("Creating as string");
		} else
		{
			Error("Unsupported type");
		}
	}
	
	void SetModel(Managed model)
	{
		m_Model = model;

		UpdateView();
	}
	
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		if ( m_Model )
			UpdateView();
		
		return true;
	}

	void DebugPrint()
	{
		Print( "variable_name:" + variable_name );
		Print( "variable_type:" + variable_type );
		Print( "variable_index:" + variable_index );
		Print( "control_name:" + control_name );
	}

	void UpdateView()
	{
		Print("EditorView::UpdateView");
		switch (m_Type)
		{
			case EditorViewType.TEXT:
			{
				string text;
				
				MultilineEditBoxWidget mebw = MultilineEditBoxWidget.Cast(m_ControlWidget);
				if (mebw)
				{
					mebw.GetText(text);
				}
			
				EditBoxWidget ebw = EditBoxWidget.Cast(m_ControlWidget);
				if (ebw)
				{
					text = ebw.GetText();
				}
				
				Print(text);
				Print(m_Model);
				DebugPrint();
				
				if (variable_type == "float")
				{
					EnScript.SetClassVar(m_Model, variable_name, variable_index, text.ToFloat());
				} else if (variable_type == "int")
				{
					EnScript.SetClassVar(m_Model, variable_name, variable_index, text.ToInt());
				} else if (variable_type == "string")
				{
					EnScript.SetClassVar(m_Model, variable_name, variable_index, text);
				}
			
				return;
			}
			case EditorViewType.CHECKBOX:
			{
				EnScript.SetClassVar(m_Model, variable_name, variable_index, CheckBoxWidget.Cast(m_ControlWidget).IsChecked());
				return;
			}
		}
	}
	
	void UpdateModel()
	{
		switch (m_Type)
		{
			case EditorViewType.TEXT:
				string text;
			
				if (variable_type == "float")
				{
					float text_float;
					EnScript.GetClassVar(m_Model, variable_name, variable_index, text_float);
					text = "" + text_float;
				} else if (variable_type == "int")
				{
					int text_int;
					EnScript.GetClassVar(m_Model, variable_name, variable_index, text_int);
					text = "" + text_int;
				} else if (variable_type == "string")
				{
					EnScript.GetClassVar(m_Model, variable_name, variable_index, text);
				}
			
				MultilineEditBoxWidget mebw = MultilineEditBoxWidget.Cast(m_ControlWidget);
				if (mebw)
				{
					mebw.SetText(text);
					return;
				}
			
				EditBoxWidget ebw = EditBoxWidget.Cast(m_ControlWidget);
				if (ebw)
				{
					ebw.SetText(text);
					return;
				}
			
				return;
			case EditorViewType.CHECKBOX:
				bool checked;
				EnScript.GetClassVar(m_Model, variable_name, variable_index, checked);
			
				CheckBoxWidget.Cast(m_ControlWidget).SetChecked(checked);
				return;
		}
	}
	
	static EditorView GetUIProperty(Widget root, Managed model, string name)
	{
		EditorView view;
		root.FindAnyWidget(name).GetScript(view);
		
		view.SetModel(model);
		
		return view;
	}
	
	static ref array< EditorView > GetUIProperties(Widget root, Managed model)
	{
		ref array< EditorView > views = new array< EditorView >();
		_GetUIProperties(root, model, views);
		return views;
	}
	
	private static void _GetUIProperties(Widget root, Managed model, inout array< EditorView > views)
	{
		EditorView view;
		root.GetScript(view);
		if (view != null)
		{
			view.SetModel(model);
			
			views.Insert(view);
		}
		
		Widget child = root.GetChildren();
		while (child != null)
		{
			_GetUIProperties(child, model, views);
			
			child = child.GetSibling();
		}
	}
};