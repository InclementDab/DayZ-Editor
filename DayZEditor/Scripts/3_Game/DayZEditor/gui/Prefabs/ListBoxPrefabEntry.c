class ListBoxPrefabEntryController<Class TValue>: Controller
{
	string Icon;
	string Caption;
	TValue Value;
}

class ListBoxPrefabEntry<Class T>: ScriptView
{
	ref ScriptInvoker Event_OnClick = new ScriptInvoker();
	ref ScriptInvoker Event_OnDoubleClick = new ScriptInvoker();
	
	void ListBoxPrefabEntry(string caption, T data, string icon = "")
	{
		GetListBoxPrefabEntryController().Caption = caption;
		GetListBoxPrefabEntryController().NotifyPropertyChanged("Caption");
		
		GetListBoxPrefabEntryController().Value = data;
		GetListBoxPrefabEntryController().NotifyPropertyChanged("Value");
		
		GetListBoxPrefabEntryController().Icon = icon;
		GetListBoxPrefabEntryController().NotifyPropertyChanged("Icon");
	}
	
	void ~ListBoxPrefabEntry()
	{
		delete Event_OnClick;
		delete Event_OnDoubleClick;
	}
	
	ListBoxPrefabEntryController<T> GetListBoxPrefabEntryController() 
	{
		return ListBoxPrefabEntryController<T>.Cast(GetController());
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Event_OnClick.Invoke(GetListBoxPrefabEntryController().Value, w, x, y, button);
		return true;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		Event_OnDoubleClick.Invoke(GetListBoxPrefabEntryController().Value, w, x, y, button);
		return true;
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