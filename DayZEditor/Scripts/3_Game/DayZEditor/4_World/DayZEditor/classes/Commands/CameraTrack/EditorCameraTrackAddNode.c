class EditorCameraTrackAddNode: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		string name = "CameraTrack" + m_Editor.GetCameraTrackManager().CameraTrackCount();
		m_Editor.GetCameraTrackManager().InsertCameraTrack(m_Editor.GetCamera(), 1.0, name);
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CINEMATIC_CAMERA_ADD_NODE";
	}
}