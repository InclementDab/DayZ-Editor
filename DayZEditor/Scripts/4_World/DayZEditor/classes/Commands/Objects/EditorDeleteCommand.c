class EditorDeleteCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorCameraTrackManagerModule manager = m_Editor.GetCameraTrackManager();
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