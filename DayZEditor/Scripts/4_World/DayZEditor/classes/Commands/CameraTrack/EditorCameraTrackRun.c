class EditorCameraTrackRun: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		if (GetEditor().GetCameraTrackManager().IsRunning()) {
			GetEditor().GetCameraTrackManager().Stop();
		} else {
			GetEditor().GetCameraTrackManager().Start();
		}
		
		return true;
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