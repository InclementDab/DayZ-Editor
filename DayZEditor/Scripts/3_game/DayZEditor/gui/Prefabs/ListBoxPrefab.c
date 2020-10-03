
class ListBoxPrefabController: Controller
{
	autoptr ObservableCollection<string> ListBoxData = new ObservableCollection<string>("ListBoxData", this);
	string SelectedListBoxItem;
	
	override void PropertyChanged(string property_name)
	{
		ScriptedViewBase view_base = GetScriptedRoot();
	}
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