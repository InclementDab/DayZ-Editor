class EditorCameraTrackRun: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorHudController hud_controller = m_Editor.GetEditorHud().GetTemplateController();
		if (hud_controller.IsCameraTrackRunning()) {
			hud_controller.CameraTrackStop();
		} else {
			hud_controller.CameraTrackStart();
		}
	}
	
	override string GetName() 
	{
		return "Run Track";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_O };
	}
}