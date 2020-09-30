class EditBoxDialogController: Controller
{
	string Text;
}

class EditBoxPrefab: ScriptView
{	
	EditBoxDialogController GetEditBoxDialogController() {
		return EditBoxDialogController.Cast(GetController());
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/prefabs/EditBoxPrefab.layout";
	}
		
	override typename GetControllerType() {
		return EditBoxDialogController;
	}
}