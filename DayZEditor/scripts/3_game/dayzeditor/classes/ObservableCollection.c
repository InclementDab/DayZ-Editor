
enum NotifyCollectionChangedAction
{
	Add,
	Remove,
	Replace,
	Move,
	Reset
};



class IObservable
{
	protected static ref ScriptInvoker CollectionChanged = new ScriptInvoker();	
	static void NotifyOnCollectionChanged(func action)
	{
		if (CollectionChanged == null) {
			CollectionChanged = new ScriptInvoker();
		}
		
		CollectionChanged.Insert(action);
	}
	
	
	protected string m_VariableName;
	void IObservable(string var_name)
	{
		m_VariableName = var_name;
	}
	
}

class ObservableDictionary<Class TKey, Class TValue>: IObservable
{
	private ref map<TKey, TValue> _data = new map<TKey, TValue>();
	
	
	bool Insert(TKey key, TValue value)
	{
		if (_data.Insert(key, value)) {
			CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Add, m_VariableName);
			return true;
		}
		
		return false;
	}
	
	void Remove(string key)
	{
		if (_data.Contains(key)) {
			_data.Remove(key);
			CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Remove, m_VariableName);
		}
	}
	
	void Clear()
	{
		_data.Clear();
		CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Reset);
	}
	
	void Set(TKey key, TValue value)
	{
		_data.Set(key, value);
		CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Replace, m_VariableName);
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

class ObservableCollection<Class TValue>: IObservable
{
	private ref array<ref TValue> _data = new array<ref TValue>();	
	
	int Insert(TValue value)
	{
		int index = _data.Insert(value);
		if (index != -1) {
			CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Add, value, m_VariableName);
		}
		
		return index;
	}
	
	
	void Remove(int index)
	{
		int count = _data.Count();
		TValue value = _data.Get(index);
		
		if (value) {
			CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Remove, value, m_VariableName);
			_data.Remove(index);
		}
		
	}
	
	void Clear()
	{
		_data.Clear();
		CollectionChanged.Invoke(this, NotifyCollectionChangedAction.Reset, m_VariableName);
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

