class EditorDeleteCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		if (m_Editor.GetEditorHud().SelectedCameraTracks.Count() > 0) {
			EditorLog.Info("Deleting %1 Camera Tracks", m_Editor.GetEditorHud().SelectedCameraTracks.Count().ToString());
			foreach (EditorCameraTrackListItem camera_track: m_Editor.GetEditorHud().SelectedCameraTracks) {
				camera_track.OnDeleteExecute(null);
			}
			
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