

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


class Observable
{
	
	
	// Called only when Collection is changed
	protected static ref ScriptInvoker CollectionChanged = new ScriptInvoker();	
	static void NotifyOnCollectionChanged(func action)
	{
		if (CollectionChanged == null)
			CollectionChanged = new ScriptInvoker();
		
		
		CollectionChanged.Insert(action);
	}
		
	
	protected string m_VariableName;
	void Observable(string var_name)
	{
		m_VariableName = var_name;
	}
	
}

class ObservableDictionary<Class TKey, Class TValue>: Observable
{
	private ref map<TKey, TValue> _data = new map<TKey, TValue>();
	
	
	bool Insert(TKey key, TValue value)
	{
		if (_data.Insert(key, value)) {
			CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Add, new Param2<TKey, TValue>(key, value), m_VariableName);
			return true;
		}
		
		return false;
	}
	
	void Remove(TKey key)
	{
		if (_data.Contains(key)) {
			TValue value = _data.Get(key);
			_data.Remove(key);
			CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Remove, new Param2<TKey, TValue>(key, value), m_VariableName);
		}
	}
	
	void Clear()
	{
		_data.Clear();
		CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Clear, null, m_VariableName);
	}
	
	void Set(TKey key, TValue value)
	{
		_data.Set(key, value);
		CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Replace, m_VariableName, new Param2<TKey, TValue>(key, value), m_VariableName);
	}
	
	TValue Get(TKey key)
	{
		return _data.Get(key);
	}
		
	TKey GetKey(int index)
	{
		return _data.GetKey(index);
	}
	
	int Count() { return _data.Count(); }
}

class ObservableCollection<Class TValue>: Observable
{
	private ref array<ref TValue> _data = new array<ref TValue>();	
	
	int Insert(TValue value)
	{
		int index = _data.Insert(value);
		if (index != -1) {
			CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Add, new Param1<TValue>(value), m_VariableName);
		}
		
		return index;
	}
	
	
	void Remove(int index)
	{
		int count = _data.Count();
		TValue value = _data.Get(index);
		
		if (value) {
			_data.Remove(index);
			CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Remove, new Param1<TValue>(value), m_VariableName);
		}
		
	}
	
	void Clear()
	{
		_data.Clear();
		CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Reset, null, m_VariableName);
	}
	
	TValue Get(int index)
	{
		TValue v = _data.Get(index);
		if (!v) {
			EditorLog.Error(string.Format("ObservableCollection.Get returned null! Index: %1 Name: %2", index, m_VariableName));
		}
		
		return v;
	}
	
	int Count() { return _data.Count(); }
	
	
	
}

