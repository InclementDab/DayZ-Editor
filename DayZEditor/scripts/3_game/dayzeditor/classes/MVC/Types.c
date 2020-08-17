

// TextWidget
typedef string TextWidgetData;

// ButtonWidget
typedef bool ButtonWidgetData;

// CheckBoxWidget
typedef bool CheckBoxWidgetData;

// SliderWidget
typedef float SliderWidgetData;



// TextListboxWidget
class TextListboxWidgetData: ObservableDictionary<string, Class>
{
	// Param1: Text [string]
	// Param2: UserData [Class]
	void Add(Widget w, Param params)
	{
		Param2<string, Class> changed_data = Param2<string, Class>.Cast(params);
		TextListboxWidget.Cast(w).AddItem(changed_data.param1, changed_data.param2, 0);
	}
	
	void Remove(Widget w)
	{
		Clear(w);
		for (int i = 0; i < Count(); i++) {
			string key = GetKey(i);
			TextListboxWidget.Cast(w).AddItem(key, Get(key), 0);
		}
	}
	
	void Reload(Widget w)
	{
		Clear(w);
		
		for (int i = 0; i < Count(); i++) {
			string key = GetKey(i);
			TextListboxWidget.Cast(w).AddItem(key, Get(key), 0);
		}
	}

	void Clear(Widget w)
	{
		TextListboxWidget.Cast(w).ClearItems();
	}
}

// WrapSpacerWidget
class WrapSpacerWidgetData: ObservableCollection<Widget>
{
	// Param1: Widget
	void Add(Widget w, Param params)
	{
		w.AddChild(Param1<Widget>.Cast(params).param1);
	}
	
	// Param1: Widget
	void Remove(Widget w, Param params)
	{
		w.RemoveChild(Param1<Widget>.Cast(params).param1);
	}
	
	void Reload(Widget w)
	{	
		Clear(w);
		
		for (int i = 0; i < Count(); i++) {
			w.AddChild(Get(i));
		}
	}
	
	void Clear(Widget w)
	{
		Widget child = w.GetChildren();
		while (child != null) {
			w.RemoveChild(child);
			child = child.GetSibling();
		}
	}
}

// XComboBoxWidget
class XComboBoxWidgetData: ObservableCollection<string> 
{
	// Param1: string
	void Add(Widget w, Param params)
	{
		XComboBoxWidget.Cast(w).AddItem(Param1<string>.Cast(params).param1);
	}
	
	void Remove(Widget w)
	{
		Clear();
		for (int i = 0; i < Count(); i++) 
			XComboBoxWidget.Cast(w).AddItem(Get(i));
	}
	
	void Reload(Widget w)
	{
		Clear(w);
		for (int i = 0; i < Count(); i++) 
			XComboBoxWidget.Cast(w).AddItem(Get(i));
	}
	
	void Clear(Widget w)
	{
		XComboBoxWidget.Cast(w).ClearAll();
	}
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
// 3: Collection name
typedef Param3<Observable, int, Param> CollectionChangedEventArgs;



// Event Args for Property Changed
// 0: Name of property changed
// 1: New property value
typedef Param2<string, Class> PropertyChangedEventArgs;

