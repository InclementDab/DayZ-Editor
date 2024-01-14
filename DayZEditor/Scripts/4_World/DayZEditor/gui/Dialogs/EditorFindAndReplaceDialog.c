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
		
		AddContent(new EditBoxPrefab("#STR_EDITOR_FIND", m_Controller, "Find"));
		AddContent(new EditBoxPrefab("#STR_EDITOR_FIND_AND_REPLACE", m_Controller, "Replace"));
		
		AddButton("#STR_EDITOR_FIND", "OnFind");
		AddButton("#STR_EDITOR_FIND_AND_REPLACE", "OnFindAndReplace");
		AddButton("#STR_EDITOR_CLOSE", DialogResult.Cancel);
	}
	
	void OnFind(ButtonCommandArgs args)
	{
		EditorFindAndReplaceCommand cmd = EditorFindAndReplaceCommand.Cast(GetEditor().CommandManager[EditorFindAndReplaceCommand]);
		array<EditorObject> found_objects = cmd.FindObjects(m_EditorFindAndReplaceDialogController.Find);
		
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
		array<EditorObject> replaced_objects = cmd.FindAndReplaceObjects(m_EditorFindAndReplaceDialogController.Find, m_EditorFindAndReplaceDialogController.Replace);
		
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