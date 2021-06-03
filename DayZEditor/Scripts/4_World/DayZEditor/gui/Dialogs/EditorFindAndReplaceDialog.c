class EditorFindAndReplaceDialogController: DialogBaseController
{
	string Find;
	string Replace;
}

class EditorFindAndReplaceDialog: EditorDialogBase
{
	protected EditorFindAndReplaceDialogController m_EditorFindAndReplaceDialogController;
	
	void EditorFindAndReplaceDialog(string title)
	{
		m_EditorFindAndReplaceDialogController = EditorFindAndReplaceDialogController.Cast(m_Controller);
		
		AddContent(new EditBoxPrefab("Find", m_Controller, "Find"));
		AddContent(new EditBoxPrefab("Replace", m_Controller, "Replace"));
		
		AddButton("Find", "OnFind");
		AddButton("Find + Replace", "OnFindAndReplace");
		AddButton("Close", DialogResult.Cancel);
	}
	
	void OnFind(ButtonCommandArgs args)
	{
		EditorFindAndReplaceCommand cmd = EditorFindAndReplaceCommand.Cast(GetEditor().CommandManager[EditorFindAndReplaceCommand]);
		EditorObjectMap found_objects = cmd.FindObjects(m_EditorFindAndReplaceDialogController.Find);
		
		foreach (int _, EditorObject editor_object: found_objects) {
			GetEditor().SelectObject(editor_object);
		}
		
		if (found_objects.Count() == 0) {
			EditorLog.Warning("Found 0 objects");
		}
		
		EditorLog.Info("Found %1 objects", found_objects.Count().ToString());
	}
	
	void OnFindAndReplace(ButtonCommandArgs args)
	{
		EditorFindAndReplaceCommand cmd = EditorFindAndReplaceCommand.Cast(GetEditor().CommandManager[EditorFindAndReplaceCommand]);
		EditorObjectMap replaced_objects = cmd.FindAndReplaceObjects(m_EditorFindAndReplaceDialogController.Find, m_EditorFindAndReplaceDialogController.Replace);
		
		foreach (int _, EditorObject editor_object: replaced_objects) {
			GetEditor().SelectObject(editor_object);
		}
		
		if (replaced_objects.Count() == 0) {
			EditorLog.Warning("Found 0 objects");
		}
		
		EditorLog.Info("Found and Replaced %1 objects", replaced_objects.Count().ToString());
	}
	
	override typename GetControllerType()
	{
		return EditorFindAndReplaceDialogController;
	}
}