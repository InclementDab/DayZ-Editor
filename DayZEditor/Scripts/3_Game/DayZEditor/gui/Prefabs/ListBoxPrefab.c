class ListBoxPrefabEntryController<Class TValue>: Controller
{
	string Caption;
	TValue Value;
}

class ListBoxPrefabEntry<Class T>: ScriptView
{
	ref ScriptInvoker Event_OnClick = new ScriptInvoker();
	ref ScriptInvoker Event_OnDoubleClick = new ScriptInvoker();
	
	void ListBoxPrefabEntry(string caption, T data)
	{
		GetListBoxPrefabEntryController().Caption = caption;
		GetListBoxPrefabEntryController().NotifyPropertyChanged("Caption");
		
		GetListBoxPrefabEntryController().Value = data;
		GetListBoxPrefabEntryController().NotifyPropertyChanged("Value");
	}
	
	ListBoxPrefabEntryController<T> GetListBoxPrefabEntryController() 
	{
		return ListBoxPrefabEntryController<T>.Cast(GetController());
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Event_OnClick.Invoke(GetListBoxPrefabEntryController().Value, w, x, y, button);
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		Event_OnDoubleClick.Invoke(GetListBoxPrefabEntryController().Value, w, x, y, button);
		return super.OnDoubleClick(w, x, y, button);
	}
	
	T GetData()
	{
		return GetListBoxPrefabEntryController().Value;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/prefabs/Listbox/EditorSelectItemList.layout";
	}
	
	override typename GetControllerType()
	{
		return (new ListBoxPrefabEntryController<T>()).Type();
	}
}

class ListBoxPrefabController<Class T>: Controller
{
	ref ObservableCollection<ref ListBoxPrefabEntry<T>> ListBoxData = new ObservableCollection<ref ListBoxPrefabEntry<T>>(this);
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		super.CollectionChanged(collection_name, args);
	}
}

class ListBoxPrefab<Class T>: ScriptView
{	
	private static const T DEFAULT_VALUE;
		
	ref ScriptInvoker Event_OnClick = new ScriptInvoker();
	ref ScriptInvoker Event_OnDoubleClick = new ScriptInvoker();
	
	ListBoxPrefabController<T> GetListBoxPrefabController() 
	{
		return ListBoxPrefabController<T>.Cast(GetController());
	}
	
	protected T m_SelectedItem;
	
	void InsertItem(string caption, T item)
	{
		ListBoxPrefabEntry<T> entry(caption, item);
		entry.Event_OnClick.Insert(EntryOnClick);
		entry.Event_OnDoubleClick.Insert(EntryOnDoubleClick);
		
		GetListBoxPrefabController().ListBoxData.Insert(entry);
	}
	
	void EntryOnClick(T entry, Widget w, int x, int y, int button)
	{
		m_SelectedItem = entry;
		Event_OnClick.Invoke(entry, w, x, y, button);
	}
	
	void EntryOnDoubleClick(T entry, Widget w, int x, int y, int button)
	{
		Event_OnDoubleClick.Invoke(entry, w, x, y, button);
	}
	
	int GetRowCount()
	{
		return GetListBoxPrefabController().ListBoxData.Count();
	}
	
	T GetSelectedItem()
	{		
		return m_SelectedItem;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/prefabs/Listbox/ListBoxWrapPrefab.layout";
	}
	
	override typename GetControllerType() 
	{
		return (new ListBoxPrefabController<T>()).Type();
	}
}