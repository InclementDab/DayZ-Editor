class EditBoxDialogController: DialogBaseController
{
	string Text;
}

class EditorEditBoxDialog: EditorDialogBase
{
	void EditorEditBoxDialog(Widget parent = null, string title = "")
	{
		AddContent(new EditBoxPrefab());
		
		AddButton("Create", DialogResult.OK);
		AddButton("Cancel", DialogResult.Cancel);
	}
	
	DialogResult ShowDialog(out string edit_data = "")
	{
		DialogResult result = super.ShowDialog();
		edit_data = EditBoxDialogController.Cast(m_Controller).Text;
		return result;
	}
	
	override typename GetControllerType() {
		return EditBoxDialogController;
	}
	
}