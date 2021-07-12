class EditorDeleteCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args) 
	{
		super.Execute(sender, args);
		EditorCameraTrackManagerModule manager = m_Editor.GetCameraTrackManager();
		if (manager.GetSelectedTracks().Count() > 0) {
			EditorLog.Info("Deleting %1 Camera Tracks", manager.GetSelectedTracks().Count().ToString());
			manager.DeleteCameraTracks(manager.GetSelectedTracks());
			return true;
		}
		
		EditorDeletedObjectMap deleted_objects = m_Editor.GetSelectedHiddenObjects();
		if (deleted_objects.Count() > 0) {
			m_Editor.UnhideMapObjects(deleted_objects);
			return true;
		}
		
		EditorObjectMap objects = m_Editor.GetSelectedObjects();
		if (objects.Count() == 0) {
			return false;
		}
		
		m_Editor.DeleteObjects(objects);	
		
		return true;	
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_DELETE";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_DELETE };
	}
}