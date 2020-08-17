
class WidgetData<Class T1>
{
	T1 value;
	string name;
	
	void WidgetData(string _name)
	{
		name = _name;
	}
	
	void Set(T1 _value)
	{
		value = _value;
		NotifyPropertyChanged(new Param2<string, Class>(name, this));
	}
	
	T1 Get()
	{
		return value;
	}
}





// TextWidget
typedef WidgetData<string> TextWidgetData;

// ButtonWidget
typedef WidgetData<bool> ButtonWidgetData;

// CheckBoxWidget
typedef WidgetData<bool> CheckBoxWidgetData;

// SliderWidget
typedef WidgetData<float> SliderWidgetData;



// TextListboxWidget
class TextListboxWidgetData: ObservableDictionary<string, Class>
{
	void DictionaryAdd(TextListboxWidget w, Param params)
	{
		Param2<string, Class> changed_data = Param2<string, Class>.Cast(params);
		w.AddItem(changed_data.param1, changed_data.param2, 0);
	}
	
	void DictionaryRemove(TextListboxWidget w)
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
	void DictionaryAdd(WrapSpacerWidget w, Param params)
	{
		w.AddChild(Param1<Widget>.Cast(params).param1);
	}
	
	void DictionaryRemove(WrapSpacerWidget w, Param params)
	{
		w.RemoveChild(Param1<Widget>.Cast(params).param1);
	}
}

// XComboBoxWidget
class XComboBoxWidgetData: ObservableCollection<string> 
{
	void DictionaryAdd(XComboBoxWidget w, Param params)
	{
		w.AddItem(Param1<string>.Cast(params).param1);
	}
	
	void DictionaryRemove(XComboBoxWidget w)
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
typedef Param4<Observable, int, Param2<Class, Class>, string> CollectionChangedEventArgs;



// Event Args for Property Changed
// 0: Name of property changed
// 1: New property value
typedef Param2<string, Class> PropertyChangedEventArgs;

