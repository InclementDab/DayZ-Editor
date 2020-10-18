
class ListBoxPrefabController<Class T>: Controller
{
	autoptr ObservableCollection<T> ListBoxData = new ObservableCollection<T>(this);
	T SelectedListBoxItem;
}

class ListBoxPrefab<Class T>: ScriptView
{	
	autoptr TextListboxWidget ListBox;
	
	ListBoxPrefabController<T> GetListBoxPrefabController() {
		return ListBoxPrefabController<T>.Cast(GetController());
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/ListBoxPrefab.layout";
	}
	
	override typename GetControllerType() {
		return (new ListBoxPrefabController<T>()).Type();
	}
}