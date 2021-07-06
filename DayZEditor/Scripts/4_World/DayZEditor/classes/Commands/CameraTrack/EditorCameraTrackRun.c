class EditorCameraTrackRun: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		if (GetEditor().GetCameraTrackManager().IsRunning()) {
			GetEditor().GetCameraTrackManager().Stop();
		} else {
			GetEditor().GetCameraTrackManager().Start();
		}
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CINEMATIC_CAMERA_RUN";
	}
	
	/*override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_O };
	}*/
}