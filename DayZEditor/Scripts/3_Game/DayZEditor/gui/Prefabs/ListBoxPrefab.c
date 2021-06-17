class ListBoxPrefabController<Class T>: ViewController
{
	ref ObservableCollection<ref ListBoxPrefabEntry<T>> ListBoxData = new ObservableCollection<ref ListBoxPrefabEntry<T>>(this);
}

class ListBoxPrefab<Class T>: ScriptView
{	
	ref ScriptInvoker Event_OnClick = new ScriptInvoker();
	ref ScriptInvoker Event_OnDoubleClick = new ScriptInvoker();
	
	ListBoxPrefabController<T> GetListBoxPrefabController() 
	{
		return ListBoxPrefabController<T>.Cast(GetController());
	}
	
	void ~ListBoxPrefab()
	{
		delete Event_OnClick;
		delete Event_OnDoubleClick;
	}
	
	protected T m_SelectedItem;
	
	void InsertItem(string caption, T item)
	{
		ListBoxPrefabEntry<T> entry(caption, item);		
		entry.SetOwner(this);
		GetListBoxPrefabController().ListBoxData.Insert(entry);
	}
	
	void ClearItems()
	{
		for (int i = 0; i < GetListBoxPrefabController().ListBoxData.Count(); i++) {
			delete GetListBoxPrefabController().ListBoxData[i];
		}
		
		GetListBoxPrefabController().ListBoxData.Clear();
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
		return "DayZEditor/gui/Layouts/prefabs/Listbox/ListBoxPrefab.layout";
	}
	
	override typename GetControllerType() 
	{
		return (new ListBoxPrefabController<T>()).Type();
	}
}