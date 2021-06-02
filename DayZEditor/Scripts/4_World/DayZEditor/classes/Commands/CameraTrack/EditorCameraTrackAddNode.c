class EditorCameraTrackAddNode: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		string name = "CameraTrack" + m_Editor.GetCameraTrackManager().CameraTrackCount();
		m_Editor.GetCameraTrackManager().InsertCameraTrack(m_Editor.GetCamera(), 1.0, name);
	}
	
	override string GetName() 
	{
		return "Add Node";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_P };
	}
}