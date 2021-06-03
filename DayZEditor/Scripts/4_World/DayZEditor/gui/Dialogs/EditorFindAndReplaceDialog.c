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
		AddButton(DialogResult.Cancel);
	}
	
	void OnFind(ButtonCommandArgs args)
	{
		EditorFindAndReplaceCommand cmd = EditorFindAndReplaceCommand.Cast(GetEditor().CommandManager[EditorFindAndReplaceCommand]);
		EditorObjectMap found_objects = cmd.FindObjects(m_EditorFindAndReplaceDialogController.Find);
		
		foreach (int _, EditorObject editor_object: found_objects) {
			GetEditor().SelectObject(editor_object);
		}
		
		EditorLog.Info("Found %1 objects", found_objects.Count().ToString());
	}
	
	void OnFindAndReplace(ButtonCommandArgs args)
	{
		EditorFindAndReplaceCommand cmd = EditorFindAndReplaceCommand.Cast(GetEditor().CommandManager[EditorFindAndReplaceCommand]);
		cmd.FindAndReplaceObjects(m_EditorFindAndReplaceDialogController.Find, m_EditorFindAndReplaceDialogController.Replace);
	}
	
	override typename GetControllerType()
	{
		return EditorFindAndReplaceDialogController;
	}
}