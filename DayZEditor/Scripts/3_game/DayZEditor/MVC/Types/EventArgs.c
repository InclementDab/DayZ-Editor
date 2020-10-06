
enum NotifyCollectionChangedAction
{
	Insert,
	InsertAt,
	Remove,
	Replace,
	Move,
	Swap,
	Clear
};

// 0: Start Index
// 1: Finish Index
typedef Param2<int, int> CollectionSwapArgs;

// Event Args for Collection Changed
// 0: Collection that was changed
// 1: Collection Changed Action
// 2: Collection Changed Index
// 3: Collection Changed Value
class CollectionChangedEventArgs
{
	Observable Source;
	NotifyCollectionChangedAction ChangedAction;	
	int ChangedIndex;
	Param ChangedValue;
	
	void CollectionChangedEventArgs(Observable source, NotifyCollectionChangedAction changedAction, int changedIndex, Param changedValue)
	{
		Source = source;
		ChangedAction = changedAction;
		ChangedIndex = changedIndex;
		ChangedValue = changedValue;
	}
}