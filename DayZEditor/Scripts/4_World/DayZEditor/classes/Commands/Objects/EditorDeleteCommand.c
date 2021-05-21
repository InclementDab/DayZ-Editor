class EditorDeleteCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		if (m_Editor.GetEditorHud().SelectedCameraTracks.Count() > 0) {
			EditorLog.Info("Deleting %1 Camera Tracks", m_Editor.GetEditorHud().SelectedCameraTracks.Count().ToString());
			GetEditor().DeleteCameraTracks(m_Editor.GetEditorHud().SelectedCameraTracks);
			return;
		}
		
		EditorObjectMap objects = m_Editor.GetSelectedObjects();
		m_Editor.DeleteObjects(objects);		
	}
		
	override string GetName() 
	{
		return "Delete";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_DELETE };
	}
}