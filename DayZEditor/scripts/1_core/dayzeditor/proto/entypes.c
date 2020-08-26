enum NotifyCollectionChangedAction {
	Add,
	Remove,
	Set,
	Move,
	Clear
};


// Event Args for Collection Changed
// 0: Observable that was changed
// 1: Collection Changed Action
// 2: Changed Index
// 3: Changed Value
typedef Param4<Class, int, int, ref Param> CollectionChangedEventArgs;



// Event Args for Property Changed
// 0: Name of property changed
// 1: New property value
typedef Param2<string, Class> PropertyChangedEventArgs;
