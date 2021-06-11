class ListBoxPrefabController<Class T>: Controller
{
	ref ObservableCollection<T> ListBoxData = new ObservableCollection<T>(this);
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		super.CollectionChanged(collection_name, args);
		
	}
}

class ListBoxPrefab<Class T>: ScriptView
{	
	//autoptr TextListboxWidget ListBox;
	private static const T DEFAULT_VALUE;
		
	ListBoxPrefabController<T> GetListBoxPrefabController() 
	{
		return ListBoxPrefabController<T>.Cast(GetController());
	}
	
	void InsertItem(T item)
	{
		
	}
	
	int GetSelectedRow()
	{
		return -1;
	}
	
	T GetSelectedItem()
	{
		return DEFAULT_VALUE;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/prefabs/ListBoxWrapPrefab.layout";
	}
	
	override typename GetControllerType() 
	{
		return (new ListBoxPrefabController<T>()).Type();
	}
}