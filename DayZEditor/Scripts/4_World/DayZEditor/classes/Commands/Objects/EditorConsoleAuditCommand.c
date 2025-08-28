#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorConsoleAuditCommand)]
#endif
class EditorConsoleAuditCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorObjectMap objects = m_Editor.GetPlacedObjects();
		foreach (EditorObject object: objects) {
			
		}
		
		auto deleted_objects = m_Editor.GetDeletedObjects();
		if (deleted_objects.Count() > 0) {
			
		}
		
		//m_Editor.GetEditorHud().ShowMessageBox();
		
		return true;
	}
	
	protected void MessageBoxCallback(DialogResult result)
	{
		
	}
	
	override string GetName()
	{
		return "Run Console Audit";
	}
}