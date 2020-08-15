
enum NotifyCollectionChangedAction
{
	Add,
	Remove,
	Replace,
	Move,
	Reset
}


class ObservableCollection<Class TKey, Class TValue>
{
	private ref map<TKey, TValue> _data;
	
	
	void ObservableCollection()
	{	
		_data = new map<TKey, TValue>(); 
	}
	
	void ObservableCollection(ref map<TKey, TValue> data)
	{
		_data = data;
	}
	
	
	bool Insert(TKey key, TValue value)
	{
		
		if (_data.Insert(key, value)) {
			PropertyChanged.Invoke("Count");
			CollectionChanged.Invoke(NotifyCollectionChangedAction.Add, value, key);
		}
			
		
		
	}
	
	void Remove(TKey key)
	{
		if (_data.Contains(key)) {
			_data.Remove(key);
			PropertyChanged.Invoke("Count");
			CollectionChanged.Invoke(NotifyCollectionChangedAction.Remove, value, key);
		}
	}
	
	void Clear()
	{
		
	}
	
	void Set(TKey key, TValue value)
	{
		
	}
	
	ref ScriptInvoker CollectionChanged = new ScriptInvoker();
	protected ref ScriptInvoker PropertyChanged = new ScriptInvoker();
	
}

