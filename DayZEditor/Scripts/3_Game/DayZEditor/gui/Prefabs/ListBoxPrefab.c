class ListBoxPrefabEntryController<Class TValue>: Controller
{
	TValue Value;
}

class ListBoxPrefabEntry<Class T>: ScriptView
{
	ref ScriptInvoker Event_OnClick = new ScriptInvoker();
	ref ScriptInvoker Event_OnDoubleClick = new ScriptInvoker();
	
	void ListBoxPrefabEntry(T data)
	{
		GetListBoxPrefabEntryController().Value = data;
	}
	
	ListBoxPrefabEntryController<T> GetListBoxPrefabEntryController() 
	{
		return ListBoxPrefabEntryController<T>.Cast(GetController());
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Event_OnClick.Invoke(this, w, x, y, button);
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		Event_OnDoubleClick.Invoke(this, w, x, y, button);
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
		
	ListBoxPrefabController<T> GetListBoxPrefabController() 
	{
		return ListBoxPrefabController<T>.Cast(GetController());
	}
	
	protected ListBoxPrefabEntry<T> m_SelectedItem;
	
	void InsertItem(T item)
	{
		ListBoxPrefabEntry<T> entry(item);
		entry.Event_OnClick.Insert(EntryOnClick);
		entry.Event_OnDoubleClick.Insert(EntryOnDoubleClick);
		
		GetListBoxPrefabController().ListBoxData.Insert(new ListBoxPrefabEntry<T>(item));
	}
	
	private void EntryOnClick(ListBoxPrefabEntry<T> entry, Widget w, int x, int y, int button)
	{
		Print("EntryONClick");
		m_SelectedItem = entry;
	}
	
	private void EntryOnDoubleClick(ListBoxPrefabEntry<T> entry, Widget w, int x, int y, int button)
	{
		Print("EntryOnDoubleClick");
	}
	
	int GetSelectedRow()
	{
		return GetListBoxPrefabController().ListBoxData.Find(m_SelectedItem);
	}
	
	int GetRowCount()
	{
		return GetListBoxPrefabController().ListBoxData.Count();
	}
	
	T GetSelectedItem()
	{
		return m_SelectedItem.GetData();
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