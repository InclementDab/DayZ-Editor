
class ListBoxPrefabController: Controller
{
	autoptr ObservableCollection<string> ListBoxData = new ObservableCollection<string>(this);
	string SelectedListBoxItem;
}

class ListBoxPrefab: ScriptView
{	
	TextListboxWidget ListBox;
	
	ref ListBoxPrefabController GetListBoxPrefabController() {
		return ListBoxPrefabController.Cast(GetController());
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/ListBoxPrefab.layout";
	}
	
	override typename GetControllerType() {
		return ListBoxPrefabController;
	}
}