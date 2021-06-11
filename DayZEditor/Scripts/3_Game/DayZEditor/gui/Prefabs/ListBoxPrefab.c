/*class ListBoxPrefabEntryController: Controller
{
	string Caption;
	DropdownListPrefabItemBase Value;
	//TValue CalculatedValue; // Used for things like SliderWidget output
		
	override void PropertyChanged(string property_name)
	{
		if (GetParent()) {
			g_Script.Call(GetParent(), "PrefabPropertyChanged", property_name);
		}
	}

}*/

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
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/items/EditorSelectItemList.layout";
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
		
		Print(args.ChangedIndex);
		Print(args.ChangedValue);
		Param1<T> value = Param1<T>.Cast(args.ChangedValue);
		Print(value);
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
		Print("InsertItem");
		Print(item);
		GetListBoxPrefabController().ListBoxData.Insert(new ListBoxPrefabEntry<T>(item));
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