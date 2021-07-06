class EditorCameraControlsCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorCameraDialog camera_dialog("Camera Properties");
		camera_dialog.ShowDialog();
	}

	override string GetName() 
	{
		return "#STR_EDITOR_CAMERA";
	}
		
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:camera_alt";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_T };
	}
}