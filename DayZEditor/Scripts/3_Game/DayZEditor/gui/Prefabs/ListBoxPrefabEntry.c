class ListBoxPrefabEntryController<Class TValue>: ViewController
{
	string Icon;
	string Caption;
	TValue Value;
}

class ListBoxPrefabEntry<Class T>: ScriptView
{
	protected ListBoxPrefab<T> m_Owner;
		
	void ListBoxPrefabEntry(string caption, T data, string icon = "")
	{
		GetListBoxPrefabEntryController().Caption = caption;
		GetListBoxPrefabEntryController().NotifyPropertyChanged("Caption");
		
		GetListBoxPrefabEntryController().Value = data;
		GetListBoxPrefabEntryController().NotifyPropertyChanged("Value");
		
		GetListBoxPrefabEntryController().Icon = icon;
		GetListBoxPrefabEntryController().NotifyPropertyChanged("Icon");
	}
	
	ListBoxPrefabEntryController<T> GetListBoxPrefabEntryController() 
	{
		return ListBoxPrefabEntryController<T>.Cast(GetController());
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		m_Owner.EntryOnClick(GetListBoxPrefabEntryController().Value, w, x, y, button);
		return true;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		m_Owner.EntryOnClick(GetListBoxPrefabEntryController().Value, w, x, y, button);
		return true;
	}
	
	void SetOwner(ListBoxPrefab<T> owner)
	{
		m_Owner = owner;
	}
	
	ListBoxPrefab<T> GetOwner()
	{
		return m_Owner;
	}
	
	T GetData()
	{
		return GetListBoxPrefabEntryController().Value;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/prefabs/Listbox/ListBoxEntryPrefab.layout";
	}
	
	override typename GetControllerType()
	{
		return (new ListBoxPrefabEntryController<T>()).Type();
	}
}