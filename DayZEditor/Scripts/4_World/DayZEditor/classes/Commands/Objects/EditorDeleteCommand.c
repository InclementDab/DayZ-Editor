class EditorDeleteCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorCameraTrackManager manager = m_Editor.GetEditorHud().GetTemplateController().GetCameraTrackManager();
		if (manager.GetSelectedTracks().Count() > 0) {
			EditorLog.Info("Deleting %1 Camera Tracks", manager.GetSelectedTracks().Count().ToString());
			manager.DeleteCameraTracks(manager.GetSelectedTracks());
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