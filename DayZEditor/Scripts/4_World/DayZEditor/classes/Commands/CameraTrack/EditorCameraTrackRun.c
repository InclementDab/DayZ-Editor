class EditorCameraTrackRun: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorHudController hud_controller = m_Editor.GetEditorHud().GetTemplateController();
		if (hud_controller.GetCameraTrackManager().IsRunning()) {
			hud_controller.GetCameraTrackManager().Stop();
		} else {
			hud_controller.GetCameraTrackManager().Start();
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