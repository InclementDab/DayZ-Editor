
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

	override void PropertyChanged(string propertyName)
	{
		switch (propertyName)
		{
			case "LeftListSelection": {
				Print("LeftList selection changed: " + LeftListSelection);
				break;
			}
		}
	}
}

*/

class ObservableCollection<Class TValue> : Observable
{
	private	ref array<autoptr TValue> _data = {};

	void ObservableCollection(Controller controller)
	{
		m_Type = TemplateType<TValue>.GetType();
	}

	void ~ObservableCollection()
	{
		delete _data;
	}

	int Insert(TValue value)
	{
		int index = _data.Insert(value);
		if (index != -1)
		{
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
	int InsertAtEx(TypeConverter typeConverter, int index)
	{
		TValue value; // scoofed
		g_Script.CallFunction(typeConverter, "GetData", value, null);
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

	int MoveIndex(int index, int moveIndex)
	{
		TValue value = _data.Get(index);
		int new_index = _data.MoveIndex(index, moveIndex);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Move, new_index, new Param1<TValue>(value)));
		return new_index;
	}

	void SwapItems(int itemA, int itemB)
	{
		_data.SwapItems(itemA, itemB);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Swap, -1, new CollectionSwapArgs(itemA, itemB)));
	}

	void Clear()
	{
		_data.Clear();
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Clear, -1, null));
	}

	TValue Get(int index)
	{
		return _data.Get(index);
	}

	override int Count()
	{
		return _data.Count();
	}

	int Find(TValue value)
	{
		Print(value);
		return _data.Find(value);
	}
};