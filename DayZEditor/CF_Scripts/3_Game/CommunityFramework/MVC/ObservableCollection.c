
/*

 Base class for Observable Types
 The design behind them is you wont need to use NotifyPropertyChanged() upon setting a variable
 They are current used to support Collections (array, set, map)

Example: 


class TestController: Controller
{
	// "LeftList" goes into Binding_Name
	ref ObservableCollection<string> LeftList; 	
	
	// "LeftListSelection" goes into Selected_Item
	string LeftListSelection; 		
	
	void TestController()
	{
		LeftList = new ObservableCollection<string>("LeftList", this);
		LeftList.Insert("Line1");
	}

	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "LeftListSelection": {
				Print("LeftList selection changed: " + LeftListSelection);
				break;
			}
		}
	}
}

*/

class Observable
{
	protected string m_VariableName;
	protected Controller m_Controller;
	
	void Observable(string variable_name, Controller controller) {
		m_VariableName = variable_name; m_Controller = controller;
	}
	
	protected void CollectionChanged(CollectionChangedEventArgs args) {
		m_Controller.NotifyCollectionChanged(m_VariableName, args);
	}
	
	string GetVariableName() {
		return m_VariableName;
	}
	
	// Abstract
	typename GetType();
	int Count();
}

class ObservableCollection<Class TValue>: Observable
{
	private ref array<ref TValue> _data = {};
	
	int Insert(TValue value)
	{
		int index = _data.Insert(value);
		if (index != -1) {
			CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Insert, index, new Param1<TValue>(value)));
		}
		
		return index;
	}
	
	int InsertAt(TValue value, int index)
	{
		int new_index = _data.InsertAt(value, index);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.InsertAt, index, new Param1<TValue>(value)));
		return new_index;
	}
	
	
	// 0: TValue value
	// 1: int index
	int InsertAtEx(TypeConverter type_converter, int index)
	{
		TValue value; // scoofed
		g_Script.CallFunction(type_converter, "GetData", value, null);
		Print(value);
		int new_index = _data.InsertAt(value, index);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.InsertAt, index, new Param1<TValue>(value)));
		return new_index;
	}
	
	void Remove(int index)
	{
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, index, new Param1<TValue>(_data.Get(index))));	
		_data.Remove(index);	
	}
	
	void Remove(TValue value)
	{
		int index = _data.Find(value);
		if (index != -1)
			Remove(index);
	}
	
	void Set(int index, TValue value)
	{
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Replace, index, new Param1<TValue>(value)));
		_data.Set(index, value);
	}
	
	int MoveIndex(int index, int move_index)
	{
		TValue value = _data.Get(index);
		int new_index = _data.MoveIndex(index, move_index);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Move, new_index, new Param1<TValue>(value)));
		return new_index;
	}
	
	void SwapItems(int item_1, int item_2)
	{
		_data.SwapItems(item_1, item_2);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Swap, -1, new CollectionSwapArgs(item_1, item_2)));
	}
	
	void Clear()
	{
		_data.Clear();
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Clear, -1, null));
	}
	
	TValue Get(int index) {		
		return _data.Get(index);
	}
	
	override int Count() { 
		return _data.Count(); 
	}
	
	int Find(TValue value) { 
		return _data.Find(value); 
	}
	
	override typename GetType() {
		return TValue;
	}
}

class ObservableSet<Class TValue>: Observable
{
	private ref set<ref TValue> _data = new set<ref TValue>();	
	
	int Insert(TValue value)
	{
		int index = _data.Insert(value);
		if (index != -1) {
			CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Add, index, new Param1<TValue>(value)));
		}
		
		return index;
	}
	
	int InsertAt(TValue value, int index)
	{
		int new_index = _data.InsertAt(value, index);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Set, index, new Param1<TValue>(value)));
		return new_index;
	}
	
	
	void Remove(int index)
	{
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, index, new Param1<TValue>(_data.Get(index))));	
		_data.Remove(index);	
	}
	
	void Remove(TValue value)
	{
		int index = _data.Find(value);
		if (index != -1)
			Remove(index);
	}
	
	void Set(int index, TValue value)
	{
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Set, index, new Param1<TValue>(value)));
		_data.Set(index, value);
	}
	
	int Move(int index, int moveindex)
	{
		int new_index = _data.MoveIndex(index, moveindex);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Move, index, new Param1<int>(new_index)));
		return new_index;
	}
	
	void Clear()
	{
		_data.Clear();
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Clear, -1, null));
	}
	
	TValue Get(int index)
	{
		TValue v = _data.Get(index);
		if (!v) {
			MVC.Error(string.Format("ObservableCollection.Get returned null! Index: %1 Name: %2", index, m_VariableName));
		}
		
		return v;
	}
	
	override int Count() { 
		return _data.Count(); 
	}
	
	int Find(TValue value) { 
		return _data.Find(value); 
	}
	
	override typename GetType() {
		return TValue;
	}
}

class ObservableDictionary<Class TKey, Class TValue>: Observable
{
	private ref map<TKey, TValue> _data = new map<TKey, TValue>();
		
	bool Insert(TKey key, TValue value)
	{
		if (_data.Insert(key, value)) {
			CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Add, new Param2<TKey, TValue>(key, value)));
			return true;
		}
		
		return false;
	}

	void Remove(TKey key)
	{
		if (_data.Contains(key)) {
			TValue value = _data.Get(key);
			_data.Remove(key);
			CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, new Param2<TKey, TValue>(key, value)));
		}
	}
	
	void Clear()
	{
		_data.Clear();
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Clear, null));
	}
	
	void Set(TKey key, TValue value)
	{
		_data.Set(key, value);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Set, new Param2<TKey, TValue>(key, value)));
	}
	

	
	TValue Get(TKey key)
	{
		return _data.Get(key);
	}
		
	TKey GetKey(int index)
	{
		return _data.GetKey(index);
	}
	
	override int Count() { 
		return _data.Count(); 
	}
	
	override typename GetType() {
		return TValue;
	}
}




