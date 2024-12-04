class EditorDeleteCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args) 
	{
		super.Execute(sender, args);

		//array<EditorCameraTrack> selected_camera_tracks = m_Editor.GetSelectedCameraTracks();
		//if (selected_camera_tracks.Count() > 0) {
		//	m_Editor.DeleteCameraTracks(selected_camera_tracks);
		//	return true;
		//}
		
		EditorDeletedObjectMap deleted_objects = m_Editor.GetSelectedHiddenObjects();
		if (deleted_objects.Count() > 0) {
			m_Editor.UnhideMapObjects(deleted_objects);
			return true;
		}
		
		EditorObjectMap objects = m_Editor.GetSelectedObjects();
		if (objects.Count() == 0) {
			return false;
		}
		
		m_Editor.GetEditorHud().SetCurrentTooltip(null);
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
	
	override Symbols GetSymbol()
	{
		return Symbols.TRASH_CAN;
	}
}