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
		
		if (EditorHiddenObject.SelectedObjects.Count() > 0) {
			m_Editor.UnhideMapObjects(EditorHiddenObject.SelectedObjects);
			return true;
		}
		
		if (EditorObject.GetSelectedEditorObjects().Count() == 0) {
			return false;
		}
		
		delete EditorHud.CurrentTooltip;
		m_Editor.DeleteObjects(EditorObject.GetSelectedEditorObjects());
		
		return true;	
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_DELETE";
	}
}