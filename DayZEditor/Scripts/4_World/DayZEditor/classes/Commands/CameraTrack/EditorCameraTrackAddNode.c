class EditorCameraTrackAddNode: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorHudController hud_controller = m_Editor.GetEditorHud().GetTemplateController();
		string name = "CameraTrack" + hud_controller.CameraTrackData.Count();
		hud_controller.InsertCameraTrack(m_Editor.GetCamera(), 1.0, name);
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