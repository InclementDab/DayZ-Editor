

// TextWidget
typedef string TextWidgetData;
class TextWidgetData: string {}

// EditBoxWidget
typedef string EditBoxWidgetData;
class EditBoxWidgetData: string {}

// MultilineEditBoxWidget
typedef TStringArray MultilineEditBoxWidgetData;
class MultilineEditBoxWidgetData: TStringArray {}

// ButtonWidget
typedef bool ButtonWidgetData;
class ButtonWidgetData: bool {}

// CheckBoxWidget
typedef bool CheckBoxWidgetData;
class CheckBoxWidgetData: bool {}

// SliderWidget
typedef float SliderWidgetData;
class SliderWidgetData: float {}

// Interface POG
class IWidgetData
{
	private Class m_Parent;
	void IWidgetData(Class parent)
	{
		m_Parent = parent;
	}
	
	void Add(Widget w, Param params)
	{
		EditorLog.Trace("IWidgetData::Add");
		GetGame().GameScript.Call(m_Parent, "_AddData", new Param2<Widget, Param>(w, params));
	}
	
	void Remove(Widget w, Param params)
	{
		EditorLog.Trace("IWidgetData::Remove");
		GetGame().GameScript.Call(m_Parent, "_RemoveData", new Param2<Widget, Param>(w, params));
	}
	
	void Set(Widget w, Param params)
	{
		EditorLog.Trace("IWidgetData::Set");
		GetGame().GameScript.Call(m_Parent, "_SetData", new Param2<Widget, Param>(w, params));
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
typedef Param2<string, Class> TStringClassPair;
class TextListboxWidgetData: ObservableCollection<TStringClassPair>
{
	
	// Param1: Index [int]
	// Param2: TStringClassPair [Param2<string, Class>]
	void AddData(Widget w, Param params)
	{
		TStringClassPair insert_params = TStringClassPair.Cast(Param2<int, TStringClassPair>.Cast(params).param2);
		TextListboxWidget.Cast(w).AddItem(insert_params.param1, insert_params.param2, 0);
	}
	
	
	void RemoveData(Widget w, Param params)
	{
		TextListboxWidget.Cast(w).RemoveRow(Param2<int, TStringClassPair>.Cast(params).param1);
	}
	
	void SetData(Widget w, Param params)
	{
		Param2<int, TStringClassPair> collection_params = Param2<int, TStringClassPair>.Cast(params);
		TStringClassPair insert_params = TStringClassPair.Cast(collection_params.param2);
		TextListboxWidget.Cast(w).SetItem(collection_params.param1, insert_params.param1, insert_params.param2, 0);
	}
	
	void ReloadData(Widget w)
	{
		ClearData(w);
		for (int i = 0; i < Count(); i++) {
			TStringClassPair data = Get(i);
			TextListboxWidget.Cast(w).AddItem(data.param1, data.param2, 0);
		}
	}


	void ClearData(Widget w)
	{
		TextListboxWidget.Cast(w).ClearItems();
	}
	
	void _AddData(Param2<Widget, Param> params) { AddData(params.param1, params.param2); }
	void _RemoveData(Param2<Widget, Param> params) { RemoveData(params.param1, params.param2); }	
	void _SetData(Param2<Widget, Param> params) { SetData(params.param1, params.param2); }
	void _ReloadData(Param1<Widget> params) { ReloadData(params.param1); }
	void _ClearData(Param1<Widget> params) { ClearData(params.param1); }
}

// WrapSpacerWidget
class WrapSpacerWidgetData: ObservableCollection<Widget>
{
	// Param1: Index
	// Param2: Widget
	void AddData(Widget w, Param params)
	{
		w.AddChild(Param2<int, Widget>.Cast(params).param2);
	}
	
	void RemoveData(Widget w, Param params)
	{
		w.RemoveChild(Param2<int, Widget>.Cast(params).param2);
	}
	
	
	void SetData(Widget w, Param params)
	{
		WrapSpacerWidget widget = WrapSpacerWidget.Cast(w);
		Param2<int, Widget> collection_params = Param2<int, Widget>.Cast(params);
		
		widget.RemoveChild(this[collection_params.param1]);
		widget.Update();
		widget.AddChildAfter(collection_params.param2, this[collection_params.param1 - 1]);
		widget.Update();
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
	void _SetData(Param2<Widget, Param> params) { SetData(params.param1, params.param2); }
	void _ReloadData(Param1<Widget> params) { ReloadData(params.param1); }
	void _ClearData(Param1<Widget> params) { ClearData(params.param1); }
}

// XComboBoxWidget
class XComboBoxWidgetData: ObservableCollection<string> 
{
	// Param1: index
	// Param2: String
	void AddData(Widget w, Param params)
	{
		XComboBoxWidget.Cast(w).AddItem(Param2<int, string>.Cast(params).param2);
	}
	
	void RemoveData(Widget w, Param params)
	{		
		XComboBoxWidget.Cast(w).RemoveItem(Param2<int, string>.Cast(params).param1);
	}
	
	void SetData(Widget w, Param params)
	{
		
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
	void _SetData(Param2<Widget, Param> params) { SetData(params.param1, params.param2); }
	void _ReloadData(Param1<Widget> params) { ReloadData(params.param1); }
	void _ClearData(Param1<Widget> params) { ClearData(params.param1); }
}

