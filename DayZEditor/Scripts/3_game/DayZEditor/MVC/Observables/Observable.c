
// Base class for all Observable Types (ObservableCollection, ObservableSet, ObservableDictionary)
class Observable
{
	// Assigned on Constructor of super
	private	typename m_Type;

	protected Controller m_Controller;

	void Observable(Controller controller)
	{
		m_Controller = controller;
	}

	protected void CollectionChanged(CollectionChangedEventArgs args)
	{
		m_Controller.NotifyCollectionChanged(args);
	}

	typename GetType()
	{
		return m_Type;
	}

	TypeConverter GetTypeConverter()
	{
		return LayoutBindingManager.GetTypeConversion(m_Type);
	}

	// Abstract
	int Count();
};