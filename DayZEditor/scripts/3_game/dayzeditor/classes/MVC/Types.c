

// TextWidget
typedef string TextWidgetData;

// ButtonWidget
typedef bool ButtonWidgetData;

// CheckBoxWidget
typedef bool CheckBoxWidgetData;

// SliderWidget
typedef float SliderWidgetData;

// Interface POG
class IWidgetData
{
	private Class m_Parent;
	void IWidgetData(Class parent)
	{
		m_Parent = parent;
	}
	
	void Add(Widget w, ref Param params)
	{
		EditorLog.Trace("IWidgetData::Add");
		GetGame().GameScript.Call(m_Parent, "_AddData", new Param2<Widget, ref Param>(w, params));
	}
	
	void Remove(Widget w, ref Param params)
	{
		EditorLog.Trace("IWidgetData::Remove");
		GetGame().GameScript.Call(m_Parent, "_RemoveData", new Param2<Widget, ref Param>(w, params));
	}
	
	void Reload(Widget w)
	{
		EditorLog.Trace("IWidgetData::Reload");
		GetGame().GameScript.Call(m_Parent, "_ReloadData", new Param1<Widget>(w));
	}
	
	void Clear(Widget w)
	{
		EditorLog.Trace("IWidgetData::Clear");
		GetGame().GameScript.Call(m_Parent, "_ClearData", new Param1<Widget>(w));
	}
}


// TextListboxWidget
class TextListboxWidgetData: ObservableDictionary<string, Class>
{
	
	// Param1: Text [string]
	// Param2: UserData [Class]
	void AddData(Widget w, Param params)
	{
		Param2<string, Class> param_data = Param2<string, Class>.Cast(params);
		TextListboxWidget.Cast(w).AddItem(param_data.param1, param_data.param2, 0);
	}
	
	void RemoveData(Widget w, Param params)
	{
		
		TextListboxWidget list_widget = TextListboxWidget.Cast(w);
		string param_text = Param2<string, Class>.Cast(params).param1;
		for (int i = 0; i < list_widget.GetNumItems(); i++) {
			string text;
			list_widget.GetItemText(i, 0, text);
			if (text == param_text) {
				list_widget.RemoveRow(i);
				return;
			}
			
		}
	}
	
	void ReloadData(Widget w)
	{
		ClearData(w);
		
		for (int i = 0; i < Count(); i++) {
			string key = GetKey(i);
			TextListboxWidget.Cast(w).AddItem(key, Get(key), 0);
		}
	}


	void ClearData(Widget w)
	{
		TextListboxWidget.Cast(w).ClearItems();
	}
	
	void _AddData(Param2<Widget, Param> params) { AddData(params.param1, params.param2); }
	void _RemoveData(Param2<Widget, Param> params) { RemoveData(params.param1, params.param2); }	
	void _ReloadData(Param1<Widget> params) { ReloadData(params.param1); }
	void _ClearData(Param1<Widget> params) { ClearData(params.param1); }
}

// WrapSpacerWidget
class WrapSpacerWidgetData: ObservableCollection<Widget>
{
	// Param1: Widget
	void AddData(Widget w, Param params)
	{
		w.AddChild(Param1<Widget>.Cast(params).param1);
	}
	
	// Param1: Widget
	void RemoveData(Widget w, Param params)
	{
		w.RemoveChild(Param1<Widget>.Cast(params).param1);
	}
	
	void ReloadData(Widget w)
	{	
		ClearData(w);
		
		for (int i = 0; i < Count(); i++) {
			w.AddChild(Get(i));
		}
	}
	
	void ClearData(Widget w)
	{
		Widget child = w.GetChildren();
		while (child != null) {
			w.RemoveChild(child);
			child = child.GetSibling();
		}
	}
	
	void _AddData(Param2<Widget, Param> params) { AddData(params.param1, params.param2); }
	void _RemoveData(Param2<Widget, Param> params) { RemoveData(params.param1, params.param2); }	
	void _ReloadData(Param1<Widget> params) { ReloadData(params.param1); }
	void _ClearData(Param1<Widget> params) { ClearData(params.param1); }
}

// XComboBoxWidget
class XComboBoxWidgetData: ObservableCollection<string> 
{
	// Param1: string
	void AddData(Widget w, Param params)
	{
		XComboBoxWidget.Cast(w).AddItem(Param1<string>.Cast(params).param1);
	}
	
	void RemoveData(Widget w, Param params)
	{
		ClearData(w);
		for (int i = 0; i < Count(); i++) 
			XComboBoxWidget.Cast(w).AddItem(Get(i));
	}
	
	void ReloadData(Widget w)
	{
		ClearData(w);
		for (int i = 0; i < Count(); i++) 
			XComboBoxWidget.Cast(w).AddItem(Get(i));
	}
	
	void ClearData(Widget w)
	{
		XComboBoxWidget.Cast(w).ClearAll();
	}
	
	void _AddData(Param2<Widget, Param> params) { AddData(params.param1, params.param2); }
	void _RemoveData(Param2<Widget, Param> params) { RemoveData(params.param1, params.param2); }	
	void _ReloadData(Param1<Widget> params) { ReloadData(params.param1); }
	void _ClearData(Param1<Widget> params) { ClearData(params.param1); }
}


enum NotifyCollectionChangedAction {
	Add,
	Remove,
	Replace,
	Move,
	Clear
};


// Event Args for Collection Changed
// 0: Observable that was changed
// 1: Collection Changed Action
// 2: Changed Value
typedef Param3<Observable, int, Param> CollectionChangedEventArgs;



// Event Args for Property Changed
// 0: Name of property changed
// 1: New property value
typedef Param2<string, Class> PropertyChangedEventArgs;

