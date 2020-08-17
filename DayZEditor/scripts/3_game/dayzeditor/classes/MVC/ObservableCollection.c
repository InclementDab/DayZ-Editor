




/*
class KeyValuePair<Class T1, Class T2>: Param
{
	
}*/


class Observable
{
	string m_VariableName;
	void Observable(string variable_name)
	{
		m_VariableName = variable_name;
	}
}

class ObservableDictionary<Class TKey, Class TValue>: Observable
{
	private ref map<TKey, TValue> _data = new map<TKey, TValue>();

	bool Insert(TKey key, TValue value)
	{
		if (_data.Insert(key, value)) {
			CollectionChanged.Invoke(m_VariableName, new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Add, new Param2<TKey, TValue>(key, value)));
			return true;
		}
		
		return false;
	}

	void Remove(TKey key)
	{
		if (_data.Contains(key)) {
			TValue value = _data.Get(key);
			_data.Remove(key);
			CollectionChanged.Invoke(m_VariableName, new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, new Param2<TKey, TValue>(key, value)));
		}
	}
	
	void Clear()
	{
		_data.Clear();
		CollectionChanged.Invoke(m_VariableName, new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Clear, null));
	}
	
	void Set(TKey key, TValue value)
	{
		_data.Set(key, value);
		CollectionChanged.Invoke(m_VariableName, new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Replace, new Param2<TKey, TValue>(key, value)));
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
			CollectionChanged.Invoke(m_VariableName, new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Add, new Param1<TValue>(value)));
		}
		
		return index;
	}
	
	
	void Remove(int index)
	{
		int count = _data.Count();
		TValue value = _data.Get(index);
		
		if (value) {
			_data.Remove(index);
			CollectionChanged.Invoke(m_VariableName, new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, new Param1<TValue>(value)));
		}
		
	}
	
	void Clear()
	{
		_data.Clear();
		CollectionChanged.Invoke(m_VariableName, new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Clear, null));
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

