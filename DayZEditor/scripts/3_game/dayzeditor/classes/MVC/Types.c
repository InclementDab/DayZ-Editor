

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
	void Add(TextListboxWidget w, Param params)
	{
		Param2<string, Class> changed_data = Param2<string, Class>.Cast(params);
		w.AddItem(changed_data.param1, changed_data.param2, 0);
	}
	
	void Remove(TextListboxWidget w)
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
	// Param1: Widget
	void Add(WrapSpacerWidget w, Param params)
	{
		w.AddChild(Param1<Widget>.Cast(params).param1);
	}
	
	// Param1: Widget
	void Remove(WrapSpacerWidget w, Param params)
	{
		w.RemoveChild(Param1<Widget>.Cast(params).param1);
	}
	
	void Reload(WrapSpacerWidget w)
	{	
		Clear(w);
		
		for (int i = 0; i < Count(); i++) {
			w.AddChild(Get(i));
		}
	}
	
	void Clear(WrapSpacerWidget w)
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
	void Add(XComboBoxWidget w, Param params)
	{
		w.AddItem(Param1<string>.Cast(params).param1);
	}
	
	void Remove(XComboBoxWidget w)
	{
		w.ClearAll();
		for (int i = 0; i < Count(); i++) 
			w.AddItem(Get(i));
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

