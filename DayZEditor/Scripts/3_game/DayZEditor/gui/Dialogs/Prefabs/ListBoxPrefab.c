
class ListBoxPrefabController: Controller
{
	autoptr ObservableCollection<string> ListBoxData = new ObservableCollection<string>("ListBoxData", this);
	string SelectedListBoxItem;
}

class ListBoxPrefab: ScriptView
{	
	TextListboxWidget ListBox;
	
	ListBoxPrefabController GetListBoxPrefabController() {
		return ListBoxPrefabController.Cast(GetController());
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/prefabs/ListBoxPrefab.layout";
	}
	
	override typename GetControllerType() {
		return ListBoxPrefabController;
	}
}